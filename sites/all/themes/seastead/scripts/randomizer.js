
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