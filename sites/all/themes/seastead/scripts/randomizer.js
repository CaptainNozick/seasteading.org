
center_left = new Array('block-views-active_topics','block-views-press_releases','block-views-recent_blog_posts');
rnd = (Math.floor((Math.random()*center_left.length)));
document.getElementById(center_left[rnd]).style.display = 'block';

center_right = new Array('block-block-3');
rnd = (Math.floor((Math.random()*center_right.length)));
document.getElementById(center_right[rnd]).style.display = 'block';