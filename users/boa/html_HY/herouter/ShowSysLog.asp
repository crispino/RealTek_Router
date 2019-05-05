<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>互联网设置</title>
<style type="text/css">
.init { color:#676767;width:670px;font-size:12px; }
</style>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<script type="text/javascript" src="/herouter/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("admin");

var pageTotal;
var count;
var pagenum;
var http_request = false;
function makeRequest(url, content) {
    http_request = false;
    if (window.XMLHttpRequest) { // Mozilla, Safari,...
        http_request = new XMLHttpRequest();
        if (http_request.overrideMimeType) {
            http_request.overrideMimeType('text/xml');
        }
    } else if (window.ActiveXObject) { // IE
        try {
            http_request = new ActiveXObject("Msxml2.XMLHTTP");
        } catch (e) {
            try {
            http_request = new ActiveXObject("Microsoft.XMLHTTP");
            } catch (e) {}
        }
    }
    if (!http_request) {
        alert('Giving up :( Cannot create an XMLHTTP instance');
        return false;
    }
    http_request.onreadystatechange = alertContents;
    http_request.open('POST', url, true);
    http_request.send(content);
}

function alertContents() {
    if (http_request.readyState == 4) {
        if (http_request.status == 200) {
			uploadLogField(http_request.responseText);
        } else {
            alert('There was a problem with the request.');
        }
    }
}

function uploadLogField(str)
{
	if(str == "-1"){
		document.getElementById("showLog").innerHTML =
		" <tr><td>Not support.\n(Busybox->\n  System Logging Utilitie ->\n	syslogd\n    Circular Buffer\n    logread</td></tr>"
	}else{
		//document.getElementById("syslog").value = str;
		var docTable = document.getElementById('showLog');
		var newDiv = document.createElement('div');
		newDiv.innerHTML = "<table><tbody>" + str + "</tbody></table>";
		var newTbody = newDiv.childNodes[0].tBodies[0];	
		var oldTbody = docTable.tBodies[0];
		docTable.replaceChild(newTbody, oldTbody);
	}
	initPageNum();

} 

function updateLog()
{
	makeRequest("/boafrm/hertsyslog", "n/a", false);
}

function initTranslation()
{
	e = document.getElementById("syslogSysLogRefresh");
	//alert(e.value);
	//e.value = _("刷新");
}

function pageInit()
{
	initTranslation();
	updateLog();
}



function refreshlogclick()
{
	updateLog();

	return true;
}

function initPageNum()
{
	var record = 16; 
	count =document.getElementById("showLog").rows.length ;//记录总数

	pageTotal = ((count+record-1)/record)|0;//总页数
	pagenum = pageTotal; 
	PTotal.innerHTML=pageTotal;

	coordinatePagenum(); 

	showhiddenRecord(pagenum);
}

function showhiddenRecord(pagenum1){ 
    var record = 16; 
    number.innerHTML=pagenum1; 
    if(pagenum1<=1){ 
        theFirstPage.innerHTML="第一页"; 
        thePrePage.innerHTML="上一页"; 
    }else{ 
        theFirstPage.innerHTML="<a href=\"javascript:firstPage()\">第一页</a>"; 
        thePrePage.innerHTML="<a href=\"javascript:prePage()\">上一页</a>"; 
    } 
    if(pagenum1>=pageTotal){ 
        theNextPage.innerHTML="下一页"; 
        theLastPage.innerHTML="最后一页"; 
    }else{ 
        theNextPage.innerHTML="<a href=\"javascript:nextPage()\">下一页</a>"; 
        theLastPage.innerHTML="<a href=\"javascript:lastPage()\">最后一页</a>"; 
    } 
    //根据当前要显示的页码取得当前面里第一条记录的号码 
    //pageBegin = (record*(pagenum1-1)+1)|0; 
	pageBegin = (record*(pagenum1-1))|0; 

    //根据当前要显示的页码取得当前面里最后一条记录的号码 
    pageEnd = record*pagenum1; 
    for(var i=0;i<count;i++){ 
        if(i>=pageBegin && i<pageEnd){ 
            document.getElementById("showLog").rows[i].style.display=""; 
        }else{ 
            document.getElementById("showLog").rows[i].style.display="none"; 
        } 
    } 
  pagenum=pagenum1; 
} 

function firstPage(){ 
    pagenum=1; 
    showhiddenRecord(pagenum); 
} 

function lastPage(){ 
    showhiddenRecord(pageTotal); 
} 

//重新整理当前页码，如果页面小于1，则赋值为1，如果大于总页数，则等于总页数 
function coordinatePagenum(){ 
     if(pagenum<1){ 
        pagenum=1; 		
    }else if(pagenum>pageTotal){ 
        pagenum=pageTotal; 
    } 
} 

function prePage(){ 
    pagenum--; 
    coordinatePagenum(pagenum); 
    showhiddenRecord(pagenum); 
} 

function nextPage(){ 
    pagenum++; 
    coordinatePagenum(pagenum); 
    showhiddenRecord(pagenum); 
} 

function gotoPage(num){ 
    coordinatePagenum(pagenum); 
    showhiddenRecord(num); 
} 

</script>

</head>
<body onload="pageInit()">
<center>
共 <span id="PTotal">?</span> 页当前第 <span id="number">1</span> 页 
<span id="theFirstPage"><a href="javascript:firstPage()">第一页</a></span> 
<span id="thePrePage"><a href="javascript:prePage()">上一页</a></span> 
<span id="theNextPage"><a href="javascript:nextPage()">下一页</a></span> 
<span id="theLastPage"><a href="javascript:lastPage()">最后一页</a></span> 
<span>

	<input type="button" value="刷新" id="syslogSysLogRefresh"
	name="refreshlog" onclick="refreshlogclick();"></span>

	<table class="init">
<tr><td>
<!-- ================= System log ================= -->
<table name="showLog" id="showLog" border="1" align="left"
width="100%"><tbody></tbody>
</table>
</td></tr></table></center> 

</body></html>
