/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CB_REMOTE_
#define _CB_REMOTE_


#include "../../sdk/cb/cbBase.h"
#include "rmx_msg.h"


class cbRemote : public cbBase {
	public:
		cbRemote(cfgBase *config, void *handle) : cbBase(config) { remote = handle; }
		virtual ~cbRemote() {}

	protected:

		virtual void cb_onDelete(cfgBase *item) {
			RmxNul( (VMSG::CALLBACK_CORE, MSG_CB_ON_DELETE, remote, item) );
		}


		virtual void cb_cfgMessage(cbMessage message, cfgBase *item, const char *text) {
			RmxNul( (VMSG::CALLBACK_CORE, MSG_CB_MESSAGE_TEXT, remote, message, item, text) );
		}

		virtual void cb_cfgMessage(cbMessage message, cfgBase *item, RMXLONG data) {
			RmxNul( (VMSG::CALLBACK_CORE, MSG_CB_MESSAGE_INT, remote, message, item, data) );
		}

		virtual void cb_cfgMessage(cbMessage message, cfgBase *item, const cfgBase *child, const cfgBase *insert=NULL) {
			RmxNul( (VMSG::CALLBACK_CORE, MSG_CB_MESSAGE_CONFIG, remote, message, (void*)item, (void*)child, (void*)insert) );
		}

		virtual void cb_cfgMessage(cbMessage message, cfgBase *item, const prmBase *param, const prmBase *insert=NULL) {
			RmxNul( (VMSG::CALLBACK_CORE, MSG_CB_MESSAGE_PARAM, remote, message, (void*)item, (void*)param, (void*)insert) );
		}
	  
/*
    virtual void cb_onDelete()                             { RmxNul( (VMSG::CALLBACK_CORE, ONDELETE,      remote          ) ); }
    virtual void cb_onNameChange (const char    *newname)  { RmxNul( (VMSG::CALLBACK_CORE, ONNAMECHANGE,  remote, newname ) ); }
    virtual void cb_onSetTooltip (const char    *newtip)   { RmxNul( (VMSG::CALLBACK_CORE, ONSETTOOLTIP,  remote, newtip  ) ); }
    virtual void cb_onSetUserData(const prmBase *userdata) { RmxNul( (VMSG::CALLBACK_CORE, ONSETUSERDATA, remote, const_cast<prmBase*>(userdata)) ); }

    virtual void cb_onInsertParam(const prmBase *param, const prmBase *insert) { RmxNul( (VMSG::CALLBACK_CORE, ONINSERTPARAM, remote, const_cast<prmBase*>(param), const_cast<prmBase*>(insert)) ); }
    virtual void cb_onRemoveParam(const prmBase *param)                        { RmxNul( (VMSG::CALLBACK_CORE, ONREMOVEPARAM, remote, const_cast<prmBase*>(param)        ) ); }
    virtual void cb_onSetParam(   const prmBase *param)                        { RmxNul( (VMSG::CALLBACK_CORE, ONSETPARAM,    remote, const_cast<prmBase*>(param)        ) ); }
    virtual void cb_onMoveParam(  const prmBase *param, const prmBase *insert) { RmxNul( (VMSG::CALLBACK_CORE, ONMOVEPARAM,   remote, const_cast<prmBase*>(param), const_cast<prmBase*>(insert)) ); }

    virtual void cb_onInsertChild(      cfgBase *child, const cfgBase *insert) { RmxNul( (VMSG::CALLBACK_CORE, ONINSERTCHILD, remote, const_cast<cfgBase*>(child), const_cast<cfgBase*>(insert)) ); }
    virtual void cb_onRemoveChild(const cfgBase *child)                        { RmxNul( (VMSG::CALLBACK_CORE, ONREMOVECHILD, remote, const_cast<cfgBase*>(child)        ) ); }
    virtual void cb_onMoveChild(  const cfgBase *child, const cfgBase *insert) { RmxNul( (VMSG::CALLBACK_CORE, ONMOVECHILD,   remote, const_cast<cfgBase*>(child), const_cast<cfgBase*>(insert)) ); }

    virtual void cb_onSetVisible(    BOOL visible)     { RmxNul( (VMSG::CALLBACK_CORE, ONSETVISIBLE,     remote, visible    ) ); }
    virtual void cb_onSetEnabled(    BOOL enable)      { RmxNul( (VMSG::CALLBACK_CORE, ONSETENABLED,     remote, enable     ) ); }
    virtual void cb_onSetExpanded(   BOOL expanded)    { RmxNul( (VMSG::CALLBACK_CORE, ONSETEXPANDED,    remote, expanded   ) ); }
    virtual void cb_onSetHighlighted(BOOL highlighted) { RmxNul( (VMSG::CALLBACK_CORE, ONSETHIGHLIGHTED, remote, highlighted) ); }
    virtual void cb_onSetSavable(    BOOL savable)     { RmxNul( (VMSG::CALLBACK_CORE, ONSETSAVABLE,     remote, savable    ) ); }
    virtual void cb_onSetReadOnly(   BOOL readonly)    { RmxNul( (VMSG::CALLBACK_CORE, ONSETREADONLY,    remote, readonly   ) ); }
*/
	private:
		void *remote;
};


#endif //_CB_REMOTE_
