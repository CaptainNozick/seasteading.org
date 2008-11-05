// $Id: imce_set_tinymce.js,v 1.1.2.6 2008/03/31 13:28:19 ufku Exp $

function imceInitiateTinyMCE(cycle) {
  if ("undefined" != typeof(window.tinyMCE)) {
    if (tinyMCE.configs) for (var i in tinyMCE.configs) {
      tinyMCE.configs[i].file_browser_callback = 'imceImageBrowser';
    }
    else if (tinyMCE.editors) for (var i in tinyMCE.editors) {//v3
      tinyMCE.editors[i].settings.file_browser_callback = 'imceImageBrowser';
    }
  }

  var cycle = (parseInt(cycle) || 0)+1;
  if (cycle < 5) setTimeout('imceInitiateTinyMCE('+ cycle +')', 2000);
}

var imceTinyWin, imceTinyField, imceTinyType, imceTinyURL;
function imceImageBrowser(field_name, url, type, win) {
  //if (type!='image') return;//work for only images
  var width = 640;
  var height = 480;
  window.open(imceBrowserURL||'/?q=imce/browse', '', 'width='+ width +', height='+ height +', resizable=1').focus();
  imceTinyWin = win;
  imceTinyField = win.document.forms[0].elements[field_name];
  imceTinyType = type;
  imceTinyURL = url;
}

$(document).ready(imceInitiateTinyMCE);
