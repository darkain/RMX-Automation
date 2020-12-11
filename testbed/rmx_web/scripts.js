function resize() {
	$('#sidebar').css( "width", $(window).width() / 100 * 20 );
	$('#sidebar').css( "height", $(window).height() - $('#sidebar').position().top - 2 );

	$('#body').css( "left", ($(window).width() / 100 * 20) + 5 );
	$('#body').css( "width", $(window).width() - $('#body').position().left - 2 );
	$('#body').css( "height", $(window).height() - $('#body').position().top - 2 );
}


function parse_sidebar_tree(tree, depth) {
	var items = [];
	$.each(tree, function(key, val) {
		if (key.substr(0, 5) == "list_"  &&  val['type'] == 'tree') {
			items.push('<li style="margin-left:' + (depth*10) + 'px" id="' + key + '" onclick="update_page(this)">' + val['name'] + '</li>');
			items = items.concat(parse_sidebar_tree(val, depth+1));
		}
	});
	return items;
}


function update_sidebar() {
	$.getJSON('/api/core/', function(data, jtext, jqxhr) {
		var items = [];
		$.each(data, function(key, val) {
			if (key.substr(0, 5) == "list_"  &&  val['type'] == 'root') {
				items = items.concat(parse_sidebar_tree(val, 0));
			}
		});
		$('<ul/>', { html: items.join('') }).appendTo('#sidebar');
  	});
}


function update_page(list_id) {
	url = '/api/core/';
	
	if (list_id) {
		list_id = $(list_id).attr("id");
		pos = list_id.indexOf('_');
		if (pos >= 0) list_id = list_id.substr( pos + 1 );
		url += list_id + '/';
	}
    
	$.getJSON(url, function(data, jtext, jqxhr) {
		$('#body').text( jqxhr.responseText );
		
		var items = [];
		$.each(data, function(data_key, data_val) {
			if (data_key.substr(0, 5) == "list_") {
				$.each(data_val, function(key, val) {
					if (key.substr(0, 5) == "list_") {
						if (val['type'] == 'label') {
							items.push('<div class="rmx-row">' + val['label'] + '</div>');
						} else if (val['type'] == 'button') {
							items.push('<div class="rmx-row">' + val['name'] + ' <button>' + val['text'] + '</button></div>');
						} else if (val['type'] == 'bool') {
							items.push('<div class="rmx-row">' + val['name'] + ' <input type="check" /></div>');
						} else if (val['type'] == 'hr') {
							items.push('<hr/>');
						}
					}
				});
			}
		});
		$('<div/>', { html: items.join('') }).appendTo('#body');
  	});
}


$(window).bind('resize', resize);
$(document).ready(function(){
	resize();
	setTimeout(resize, 1000);
	update_sidebar();
});


function dump(obj) {
    var out = '';
    for (var i in obj) {
		if (typeof obj[i] == "object") {
			out += i + ": " + dump(obj[i]) + "\n";
		} else {
			out += i + ": " + obj[i] + "\n";
		}
    }

	return out;
}

function getAttributeByIndex(obj, index){
  var i = 0;
  for (var attr in obj){
    if (index === i){
      return obj[attr];
    }
    i++;
  }
  return null;
}
