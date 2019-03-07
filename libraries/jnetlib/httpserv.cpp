/*
** JNetLib
** Copyright (C) 2001 Nullsoft, Inc.
** Author: Justin Frankel
** File: httpserv.cpp - JNL HTTP GET/POST serving implementation
** License: see jnetlib.h
**
** This class just manages the http reply/sending, not where the data 
** comes from, etc.
*/

#include "netinc.h"
#include "util.h"

#include "httpserv.h"

/*
  States for m_state:
    -1 error (connection closed, etc)
    0 not read request yet.
    1 reading headers
    2 headers read, have not sent reply
    3 sent reply
    4 closed
*/

JNL_HTTPServ::JNL_HTTPServ(JNL_Connection *con)
{
  m_con=con;
  m_state=0;
  m_reply_ready=0;
}

JNL_HTTPServ::~JNL_HTTPServ()
{
  delete m_con;
}

int JNL_HTTPServ::run()
{ // returns: < 0 on error, 0 on connection close, 1 if reading request, 2 if reply not sent, 3 if reply sent, sending data.
  int cnt=0;
run_again:
  m_con->run();
  if (m_con->get_state()==JNL_Connection::STATE_ERROR)
  {
    seterrstr(m_con->get_errstr());
    return -1;
  }
  if (m_con->get_state()==JNL_Connection::STATE_CLOSED) return 4;

  if (m_state == 0)
  {
    if (m_con->recv_lines_available()>0)
    {
		{
			int bytes = m_con->recv_bytes_available();
			char *buf = (char*)malloc(bytes + 1);
			m_con->recv_line(buf, bytes);
			buf[bytes] = 0;
			m_recv_request = buf;
			free(buf);
		}

	  char *buf   = (char*)(const char*)m_recv_request;
	  char *start = buf;
	  buf += m_recv_request.length();

      while (buf >= start && *buf != ' ') buf--;
      if (strncmp(buf+1,"HTTP",4) || (strncmp(start,"GET",3) && strncmp(start,"POST",4)))
      {
        seterrstr("malformed HTTP request");
        m_state=-1;
      }
      else
      {
        m_state=1;
        cnt=0;
		int pos = m_recv_request.find('?');

        if (buf >= start) buf[0] = buf[1] = 0;

        if (pos >= 0)
        {
		  buf = start + pos;
          *buf++ = 0; // change &'s into 0s now.
          int stat = 1;
          while (*buf) 
          {
            if (*buf == '&' && !stat) { stat=1; *buf=0; }
            else stat=0;
            buf++;
          }
        }
      }
    }
    else if (!cnt++) goto run_again;
  }
  if (m_state == 1)
  {
    if (!cnt++ && m_con->recv_lines_available()<1) goto run_again;
    while (m_con->recv_lines_available()>0)
    {
      char buf[4096];
      buf[0]=0;
      m_con->recv_line(buf,4096);
      if (!buf[0]) {
        while(true) {
          m_con->run();
          int n=m_con->recv_bytes_available();
          if(n > 0) {
			  char *tmp = (char*) malloc(n+1);
			  if (tmp) {
				  m_con->recv_bytes(tmp, n);
				  tmp[n] = 0;
				  m_postdata += tmp;
				  free(tmp);
			  }
          } else {
            break;
          }
        }

        m_state=2; break; 
      }

	  m_recvheaders += buf;

    }
  }
  if (m_state == 2)
  {
    if (m_reply_ready)
    {
      // send reply
      if (m_reply_string.length() > 0) {
		  m_con->send_string(m_reply_string);
      } else {
		  m_con->send_string("HTTP/1.1 200 OK");
      }
      m_con->send_string("\r\n");

	  if (m_reply_headers.length() > 0) {
		  m_con->send_string(m_reply_headers);
	  }
      m_con->send_string("\r\n");

      m_state=3;
    }
  }
  if (m_state == 3)
  {
    // nothing.
  }

  return m_state;
}

const char *JNL_HTTPServ::get_request_file() const
{
  // file portion of http request
	if (m_recv_request.length() < 1) return NULL;
	int pos = m_recv_request.find(' ');
	if (pos > 0) return ((const char*)m_recv_request) + pos + 1;
	return 0; //TODO: trim extra spaces in this function!
}

const char *JNL_HTTPServ::get_request_parm(CBString parmname) const // parameter portion (after ?)
{
  const char *t=m_recv_request;
  while (*t) t++;
  t++;
  while (*t)
  {
    while (*t == '&') t++;
	if (!_strnicmp(t, parmname, parmname.length()) && t[parmname.length()] == '=')
    {
		return t + parmname.length() + 1;
    }
    t += strlen(t) + 1;
  }
  return NULL;
}

const char *JNL_HTTPServ::getheader(const char *headername) const
{
  const char *ret = NULL;
  if (strlen(headername) < 1 || m_recvheaders.length() < 1) return NULL;
  char *buf=(char*)malloc(strlen(headername)+2);
  strcpy(buf,headername);
  if (buf[strlen(buf)-1]!=':') strcat(buf,":");
  const char *p=m_recvheaders;
  while (*p)
  {
    if (!strnicmp(buf,p,strlen(buf)))
    {
      ret=p+strlen(buf);
      while (*ret == ' ') ret++;
      break;
    }
    p+=strlen(p)+1;
  }
  free(buf);
  return ret;
}

void JNL_HTTPServ::set_reply_string(const char *reply_string) // should be HTTP/1.1 OK or the like
{
  m_reply_string = reply_string;
}

void JNL_HTTPServ::set_reply_header(const char *header) // "Connection: close" for example
{
	m_reply_headers += header;
	m_reply_headers += "\r\n";
}
