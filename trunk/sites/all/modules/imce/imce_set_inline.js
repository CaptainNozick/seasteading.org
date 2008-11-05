// $Id: imce_set_inline.js,v 1.1.2.7 2008/03/15 02:23:54 ufku Exp $
if (Drupal.jsEnabled) {
  $(imceInitiateInline);
}

var imceActiveTextarea, imceActiveType;
function imceInitiateInline() {
  $('a.imce-insert-inline').each( function () {
    $(this.parentNode).css('display', 'block');
    $(this).unbind('click').click(function() {
      imceActiveTextarea = $('#'+this.name.split('-IMCE-')[0]).get(0);
      imceActiveType = this.name.split('-IMCE-')[1];
      window.open(this.href, '_imce_', 'width=640, height=480, resizable=1');
      return false;
    });
  });
}

//custom callback. hook:ImceFinish
function _imce_ImceFinish(path, w, h, s, imceWin) {
  var basename = path.substr(path.lastIndexOf('/')+1);
  var type = imceActiveType=='link' ? 'link' : (w&&h ? 'image' : 'link');
  var html = type=='image' ? ('<img src="'+ path +'" width="'+ w +'" height="'+ h +'" alt="'+ basename +'" />') : ('<a href="'+ path +'">'+ basename +' ('+ s +')</a>');
  imceInsertAtCursor(imceActiveTextarea, html, type);
  imceWin.close();
  imceActiveType = null;
}

//insert html at cursor position
function imceInsertAtCursor(field, txt, type) {
  field.focus();
  if ('undefined' != typeof(field.selectionStart)) {
    if (type == 'link' && (field.selectionEnd-field.selectionStart)) {
      txt = txt.split('">')[0] +'">'+ field.value.substring(field.selectionStart, field.selectionEnd) +'</a>';
    }
    field.value = field.value.substring(0, field.selectionStart) + txt + field.value.substring(field.selectionEnd, field.value.length);
  }
  else if (document.selection) {
    if (type == 'link' && document.selection.createRange().text.length) {
      txt = txt.split('">')[0] +'">'+ document.selection.createRange().text +'</a>';
    }
    document.selection.createRange().text = txt;
  }
  else {
    field.value += txt;
  }
}
