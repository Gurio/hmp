var home 	= 	"home";
var cv 		= 	"cv";
var contact = 	"contact";
var about 	= 	"about";
var serve 	= 	"serve";

var main_sections=	[home, cv, contact, about]

var main 	= 	"main";
var stat 	= 	"stat";

function renderElementById(page, path, elem_id)
{
    var xmlHttp = null;

    xmlHttp = new XMLHttpRequest();
    xmlHttp.open( "GET", page, false );
    xmlHttp.send( null );
    document.getElementById(elem_id).innerHTML = xmlHttp.responseText;
    if (elem_id == main){
	    main_sections.forEach(function(entry) {
	    	document.getElementById(entry).className = (entry == path) ? "active" : "";
		});
	}
    
}

function lastURIfragment()
{
	if ( window.location.href.indexOf("#") == -1) 
		return home;
  	URI = window.location.href.split("#");
  	return URI[URI.length - 1]
}

function reloadContents (id){
	window.location.href = document.getElementById(id + "-ref").href;
	loadContents ();
}

function loadContents (){
	frag = lastURIfragment();
	renderElementById(frag + ".html", frag, main);
	if (frag == home){
		renderElementById(serve, serve, stat);
	}
}