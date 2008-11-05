// $Id: imce_set_fck.js,v 1.1.2.6 2008/03/31 13:28:19 ufku Exp $

function imceInitiateFCK(cycle) {
  if ("undefined" != typeof(window.FCKeditorAPI)) {
    $.each(FCKeditorAPI.__Instances, imceSetFCK);
  }
  else if ("undefined" != typeof(window.FCKeditor_OpenPopup)) {
    $('textarea').each(function () {
      var obj = 'oFCKeditor_'+ this.id.replace(/\-/g, '_');
      if ("undefined" != typeof(window[obj])) {
        imceSetFCK.call(window[obj]);
      }
    });
    return;
  }

  var cycle = (parseInt(cycle) || 0)+1;
  if (cycle < 5) setTimeout('imceInitiateFCK('+ cycle +')', 2000);
}

function imceSetFCK() {
  var types = {Image: 1, Link: 1, Flash: 1};
  var props = {Browser: true, BrowserURL: imceBrowserURL || '/?q=imce/browse'};
  for (var type in types) for (var prop in props){
    this.Config[type + prop] = props[prop];
  }
}

$(document).ready(imceInitiateFCK);
