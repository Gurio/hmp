var home 	= 	"home";
var cv 		= 	"cv";
var contact 	= 	"contact";
var about 	= 	"about";
var serve 	= 	"serve";

var main_sect	=	[home, cv, contact, about]

var main 	= 	"main";
var stat 	= 	"stat";

function renderElementById(elem_id, path, page)
{
    if(typeof(page)==='undefined') page = path;
    
    var xmlHttp = null;
    xmlHttp = new XMLHttpRequest();
    xmlHttp.open( "GET", page, false );
    xmlHttp.send( null );
    document.getElementById(elem_id).innerHTML = xmlHttp.responseText;
    if (elem_id == main){
	    main_sect.forEach(function(entry) {
	    	document.getElementById(entry).className = (entry == path) ? "active" : "";
		});
	}
    
}

function lastURIfragment()
{
	URI = window.location.href;
	if ( URI.indexOf("#") == -1) 
		return home;
  	URI_split = URI.split("#");
  	return URI_split[URI_split.length - 1]
}

function reloadContents (id){
	window.location.href = document.getElementById(id + "-ref").href;
	loadContents ();
}

function loadContents (){
	frag = lastURIfragment();
	renderElementById(main, frag, frag + ".html");
	if (frag == home){
		renderElementById(stat, serve);
	}
}
