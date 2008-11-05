document.write('<script type="text/javascript" src="' + BASE_URL + 'modules/img_assist/img_assist_textarea.js"></script>');

setTimeout("InitFCKeditorImgAssist();", 1000);

function InitFCKeditorImgAssist() {
  var oldInsertToEditor = insertToEditor;

  insertToEditor = function(content) {

    //FCKeditor enabled and running == textarea not displayed
    if ( myTextarea.style.display == 'none' ) {
      var opener = window.opener;
      if (opener.fckLaunchedJsId)
      for( var i = 0 ; i < opener.fckLaunchedJsId.length ; i++ ) {
        if ( opener.fckLaunchedTextareaId[i] == myTextarea.id ) {
          var oEditor = opener.FCKeditorAPI.GetInstance( opener.fckLaunchedJsId[i] );
          if (oEditor.EditMode == opener.FCK_EDITMODE_WYSIWYG) {
            oEditor.InsertHtml(content) ;
          }
          else {
            alert('Inserting image into FCKeditor is allowed only in WYSIWYG mode');
          }
        }
      }
      cancelAction();
      return false;
    }

    oldInsertToEditor(content);
  };
}
