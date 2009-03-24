// Bottom left content box

currentitem = 1;
totalitems = 4;
if (typeof(jQuery) == 'function') { //well, just in case...
	if ($('#center_left_inner').length) {
		totalitems = $('#center_left_inner div.block').length;
	}
}
itemwidth = 495;

allowplaying = true;

document.getElementById('total-items').innerHTML = totalitems;

function nextItem(clicked) {
	if (currentitem < totalitems) {
		for (i=1; i<=itemwidth; i++) {
			setTimeout("document.getElementById('center_left').scrollLeft += 1;",i);
		}
		currentitem++;
		document.getElementById('current-item').innerHTML = currentitem;
	}
	if (clicked) allowplaying = false;
}

function prevItem(clicked) {
	if (currentitem > 1) {
		for (i=1; i<=itemwidth; i++) {
			setTimeout("document.getElementById('center_left').scrollLeft -= 1;",i);
		}
		currentitem--;
		document.getElementById('current-item').innerHTML = currentitem;
	}
	if (clicked) allowplaying = false;
}

rnd = (Math.floor((Math.random()*totalitems)));
document.getElementById('center_left').scrollLeft = itemwidth*rnd;
currentitem = rnd+1;
document.getElementById('current-item').innerHTML = currentitem;

setInterval("changeItem();",15000);

function changeItem() {
	if (allowplaying) {
		rnd = currentitem;
		while (rnd == currentitem) {
			rnd = (Math.floor((Math.random()*totalitems))) + 1;
		}
		diff = currentitem - rnd;
		if (diff < 0) {
			for (i=0; i>=diff; i--) {
				nextItem(false);
			}
		}
		if (diff > 0) {
			for (i=0; i<=diff; i++) {
				prevItem(false);
			}
		}
	}
}

// Banner

setInterval("changeBanner();",30000);

totalbanners = 15;

function changeBanner() {
	rndbanner = (Math.floor((Math.random()*totalbanners)));
	rndbanner++;
	newbanner = new Image();
	newbanner.onload = function() {
		el = document.getElementById("banner");
		el.style.backgroundImage = "url('/sites/all/themes/seastead/images/banners/" + rndbanner + ".jpg')";
	}
	newbanner.src = '/sites/all/themes/seastead/images/banners/' + rndbanner + '.jpg';
}