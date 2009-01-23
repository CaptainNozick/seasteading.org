// $Id: fckeditor.utils.js,v 1.1.4.21 2008/12/12 20:28:45 wwalc Exp $
var fckIsRunning = new Array;
var fckIsLaunching = new Array;
var fckLaunchedTextareaId = new Array;
var fckLaunchedJsId = new Array;
var fckIsIE = ( /*@cc_on!@*/false ) ? true : false ;

function Toggle( js_id, textareaID, textTextarea, TextRTE,  xss_check )
{
	var eFCKeditorDiv	= document.getElementById( 'fck_' + js_id ) ;

	if (!fckIsRunning[js_id])
	{
		if (!fckIsLaunching[js_id])
		{
			//display is set to '' at this stage because of IE 800a025e bug
			if (fckIsIE)
			eFCKeditorDiv.style.display = '' ;
			fckIsLaunching[js_id] = true;
			$(".img_assist-button").hide();
			
      if (xss_check && $('#' + textareaID).attr('class').indexOf("filterxss2") != -1) {
        $.post(Drupal.settings.basePath + 'index.php?q=fckeditor/xss', {
            text: $('#' + textareaID).val(),
            'filters[]': Drupal.settings.fckeditor_filters[js_id]
          }, 
          function(text) {
            $('#' + textareaID).val(text);
            $('#' + js_id).val(text);
            window[js_id].ReplaceTextarea();
          }
        );
      }
      else {
        eval(js_id + '.ReplaceTextarea();');
      }
		}
		setTimeout("Toggle('" + js_id + "','" + textareaID + "','" + textTextarea + "','" + TextRTE + "'," + xss_check + ");",1000);
		return ;
	}

	var oEditor ;

	if ( typeof( FCKeditorAPI ) != 'undefined' )
	oEditor = FCKeditorAPI.GetInstance( js_id ) ;
	// Get the _Textarea and _FCKeditor DIVs.
	var eTextarea	= document.getElementById( textareaID ) ;
	var eFCKeditor	= document.getElementById( js_id ) ;

	// If the _Textarea DIV is visible, switch to FCKeditor.
	if ( eTextarea.style.display != 'none' )
	{
	  if (typeof( imceInitiateFCK ) != 'undefined') 
	    imceInitiateFCK();
		document.getElementById('switch_' + js_id).innerHTML = textTextarea;

		// Switch the DIVs display.
		eFCKeditorDiv.style.display = '' ;

		// This is a hack for Gecko 1.0.x ... it stops editing when the editor is hidden.
		if ( oEditor && !document.all )
		{
			if ( oEditor.EditMode == FCK_EDITMODE_WYSIWYG )
			oEditor.MakeEditable() ;
		}
		
		if ( eTextarea.value.length ) {
			// Set the current text in the textarea to the editor.
			oEditor.SetHTML( eTextarea.value, false ) ;
		}
		eTextarea.style.display = 'none' ;
		$(".img_assist-button").hide();
	}
	else
	{
		document.getElementById('switch_' + js_id).innerHTML = TextRTE;

		// Set the textarea value to the editor value.
		$('#' + textareaID).val(oEditor.GetHTML(true)) ;
		$(".img_assist-button").show();

		// Switch the DIVs display.
		eTextarea.style.display = '' ;
		eFCKeditorDiv.style.display = 'none' ;
	}
}

function CreateToggle(elId, jsId, fckeditorOn)
{
	var ta = document.getElementById(elId);
	var ta2 = document.getElementById('fck_' + jsId);
	ta2.value = ta.value;
	ta.parentNode.insertBefore(ta2, ta);
	if (fckeditorOn)
	ta.style.display = 'none';
	else
	ta2.style.display = 'none';
}

// The FCKeditor_OnComplete function is a special function called everytime an
// editor instance is completely loaded and available for API interactions.
function FCKeditor_OnComplete( editorInstance )
{
	fckIsRunning[editorInstance.Name] = true ;
	fckLaunchedTextareaId.push(editorInstance.Config['TextareaID']) ;
	fckLaunchedJsId.push(editorInstance.Name) ;

	// Enable the switch button. It is disabled at startup, waiting the editor to be loaded.
	var oElem = document.getElementById('switch_' + editorInstance.Name);
	if (oElem != null) {
		oElem.style.display = '';
	}
	// If the textarea isn't visible update the content from the editor.
	  $(editorInstance.LinkedField.form).submit(function() {
		for( var i = 0 ; i < fckLaunchedJsId.length ; i++ ) {
			if ( document.getElementById( fckLaunchedTextareaId[i] ).style.display == 'none' )
			document.getElementById( fckLaunchedTextareaId[i] ).value = FCKeditorAPI.GetInstance( fckLaunchedJsId[i] ).GetXHTML(true) ;
		}
	});

	//Img_Assist integration
	IntegrateWithImgAssist();
}

function IntegrateWithImgAssist()
{
	var link = document.getElementsByTagName("a");
	for (var i = 0; i < link.length; i++) {
		cl = link[i].className;
		if ( cl == "img_assist-link") {
			link[i].href = link[i].href.replace("/load/textarea", "/load/fckeditor");
		}
	}
}

function FCKeditorReplaceTextarea(textarea_id, oFCKeditor, xss_check)
{
  if ($('#' + oFCKeditor.Config['TextareaID']).length === 0) {
    return;
  }
  $(".img_assist-button").hide();
  if (xss_check && $('#' + oFCKeditor.Config['TextareaID']).attr('class').indexOf("filterxss") != -1) {
    $.post(Drupal.settings.basePath + 'index.php?q=fckeditor/xss', {
      text: $('#' + textarea_id).val(),
      'filters[]': Drupal.settings.fckeditor_filters[textarea_id]
      }, 
      function(text) {
        $('#' + textarea_id).val(text);
        oFCKeditor.ReplaceTextarea();
      }
    );
  }
  else {
    oFCKeditor.ReplaceTextarea();
  }
}

// #248146
if (Drupal.jsUpload) {
Drupal.jsUpload.prototype.onerror = function (error) {
  if (error != 'Unspecified error') {
  alert('An error occurred:\n\n'+ error);
  }
  // Remove progressbar
  $(this.progress.element).remove();
  this.progress = null;
  // Undo hide
  $(this.hide).css({
    position: 'static',
    left: '0px'
  });
}
}
