
function insert_and_eval( elemid, content )
{
  var elem = document.getElementById( elemid );
  elem.innerHTML = content;
  var scripts = elem.getElementsByTagName("script");
  for(var i=0; i < scripts.length; i++)
  {
    eval(scripts[i].innerHTML || scripts[i].text); // .text is necessary for IE.
  }
}

function do_ajax( ajax_url, elem_id, start_text, disp_progress, inprog_text, endfunc, failtext, failfunc )
{
  req = null;
  if (elem_id && start_text) document.getElementById(elem_id).innerHTML = start_text;
  if (window.XMLHttpRequest)
  {
    req = new XMLHttpRequest();
    if (req.overrideMimeType)
    {
      req.overrideMimeType('text/xml');
    }
  }
  else if (window.ActiveXObject)
  {
    try {
      req = new ActiveXObject("Msxml2.XMLHTTP");
    } catch (e)
    {
      try {
          req = new ActiveXObject("Microsoft.XMLHTTP");
      } catch (e) {
        alert("Your browser does not support AJAX - please use a different browser.");
        return;
      }
    }
  }
  try {
    req.onprogress = function( e )
    {
        if (elem_id && disp_progress) document.getElementById(elem_id).innerHTML = req.responseText;
    }
  } catch (e) { if (elem_id && inprog_text) document.getElementById(elem_id).innerHTML = inprog_text; }
  req.onreadystatechange = function()
  {
    if(req.readyState == 4)
    {
      if(req.status == 200)
      {
        if (elem_id) insert_and_eval( elem_id, req.responseText );
        if (endfunc) endfunc(req, elem_id);
      }
      else
      {
        if (elem_id && failtext) document.getElementById(elem_id).innerHTML = failtext + " : " + req.statusText;
        if (failfunc) failfunc(req, elem_id);
      }
    }
  };
  req.open("GET", ajax_url, true);
  req.send(null);
}

