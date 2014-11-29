var home 	= 	"home";
var cv 		= 	"cv";
var contact = 	"contact";
var about 	= 	"about";
var serve 	= 	"serve";

var sections=	[home, cv, contact, about, serve]

var main 	= 	"main";
var stat 	= 	"stat";

function renderElementById(path,elem_id)
{
    var xmlHttp = null;

    xmlHttp = new XMLHttpRequest();
    xmlHttp.open( "GET", path, false );
    xmlHttp.send( null );
    document.getElementById(elem_id).innerHTML = xmlHttp.responseText;
    sections.forEach(function(entry) {
	    document.getElementById(entry).className = (entry == path) ? "active" : "";
	});
    
}

