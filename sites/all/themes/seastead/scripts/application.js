/* Emulate li:hover in menu for IE6 (although jQuery's browser detection sucks, so most probably IE7 will catch on too - but it should work ok anyway) */
$(document).ready(function(){
	if ($.browser.msie) {
		$('#block-menu-2 .content > ul.menu li ul.menu').each(function(i,val){ $(val).css('display','none') });
		$('#block-menu-2 .content > ul.menu li:has(ul.menu)').each(function(i,val){
			$(val).bind('mouseenter',function(e){
				$('ul.menu:first',this).css('display','block');
				$(function() {
					var zIndexNumber = 1000;
					$('div').each(function() {
						$(this).css('zIndex', zIndexNumber);
						zIndexNumber -= 10;
					});
				});
			});
			$(val).bind('mouseleave',function(e){
				$('ul.menu:first',this).css('display','none');
			});
		});
	}
});