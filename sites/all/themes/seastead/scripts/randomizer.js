
// Bottom left content box

currentitem = 1;
totalitems = 3;
itemwidth = 495;

function nextItem() {
	if (currentitem < totalitems) {
		for (i=1; i<=itemwidth; i++) {
			setTimeout("document.getElementById('center_left').scrollLeft += 1;",i);
		}
		currentitem++;
		document.getElementById('current-item').innerHTML = currentitem;
	}
}

function prevItem() {
	if (currentitem > 1) {
		for (i=1; i<=itemwidth; i++) {
			setTimeout("document.getElementById('center_left').scrollLeft -= 1;",i);
		}
		currentitem--;
		document.getElementById('current-item').innerHTML = currentitem;
	}
}

rnd = (Math.floor((Math.random()*totalitems)));
document.getElementById('center_left').scrollLeft = itemwidth*rnd;
currentitem = rnd+1;
document.getElementById('current-item').innerHTML = currentitem;

function changeItem() {
	rnd = currentitem;
	while (rnd == currentitem) {
		rnd = (Math.floor((Math.random()*totalitems))) + 1;
	}
	diff = currentitem - rnd;
	if (diff < 0) {
		for (i=0; i>=diff; i--) {
			nextItem();
		}
	}
	if (diff > 0) {
		for (i=0; i<=diff; i++) {
			prevItem();	
		}
	}
	document.getElementById('current-item').innerHTML = rnd;
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
		changeItem();
	}
	newbanner.src = '/sites/all/themes/seastead/images/banners/' + rndbanner + '.jpg';
}