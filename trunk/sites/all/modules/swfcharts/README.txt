$Id: README.txt,v 1.4 2007/10/06 18:59:54 hlslaughter Exp $

NOTE

There has been some criticism of this module due to the fact that it relies
on proprietary code. However, this software is available for free, and it
also generates some of the nicest looking graphs I've ever seen on the web.
So rather than being "pure" and providing dodgy, pixelated images using
something like gnuplot, I chose to use SWF/Charts. 

DESCRIPTION

This is a wrapper for the XML/SWF Charts package available at
http://www.maani.us/xml_charts/

It allows for generation of very attractive charts and graphs without having
to install special graphics libraries on the server. Because the graphics are
generated client-side via Flash, there is not the additional load on the server
that comes with generating images server side with GD or jpgraph.

It provides an API for integration with other modules as well as a 'content
filter' for inline useage.

See sample images at http://www.maani.us/xml_charts/index.php?menu=Gallery

INSTALLATION

This module requires 3rd party libraries. For details, see INSTALL.txt.

USAGE

This module provides a single function which should be called as follows:

module_invoke('swfcharts', 'chart', $data, $config);

This will return a themed chart.

$data will contain a fairly elaborate data structure which you will define
according to the API details located at:
  http://www.maani.us/charts/index.php?menu=Reference

Detailed examples are provided at:
  http://www.maani.us/charts/index.php?menu=Gallery

$config is optional and will use defaults from admin/settings/swfcharts if
not supplied. 

