<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" /> 
<title>防火墙管理URLfilter</title>
<!--<link href="css/style_main.css" rel="stylesheet" type="text/css" />-->
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="js/herouter.js"></script>
<script type="text/javascript">
var http_request = false;

var DelControlList = new Array();
var DelAllList = new Array();

function makeRequest(url, content, handler) {
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
	http_request.onreadystatechange = handler;
	if (document.all)//IE
		http_request.open('POST', url, true);
	else
		http_request.open('POST', url, false);
	http_request.send(content);
}

function addnewURL()
{
	document.getElementById("MACadd").style.display = "";
}

function parseAllData()
{
	//var enable = "<!--#exec cmd=\"web 2860 nvram URLFilterEn\"-->";
	var enable;
	if ( <% getHeInfo("urlfilterEnable"); %> ){
	    if ( <% getHeInfo("currentURLfiltermode"); %> ){
			enable = 1;
		}
		else
		    enable = 2;
	}
	else 
		enable = 0;
	
	document.MACBasicSettings.apselect_0.value = enable;

	var switchcode=0;
	if(document.MACBasicSettings.apselect_0.value == 0){
		$("#state").addClass("switchoff");
		$(".swindow").css("display","none");
		$(".macinfo").css("display","none");
	}	
	else if(document.MACBasicSettings.apselect_0.value == 1){
		$("#state").removeClass("switchoff");
		$(".swindow").css("display","");
		//$(".macinfo").css("display","");
		$("#btndown").removeClass("downon");
		$("#btndown").addClass("downoff");
		$("#disptxt").removeClass("opst2");
		$("#btnup").removeClass("upoff");
		$("#btnon").addClass("upon");
	  document.getElementById("allow").style.display= "";
	  document.getElementById("reject").style.display= "none"
	}	
	else if(document.MACBasicSettings.apselect_0.value == 2){
		$("#state").removeClass("switchoff");
		$(".swindow").css("display","");
		//$(".macinfo").css("display","");
		$("#btnup").removeClass("upon");
		$("#btnup").addClass("upoff");
		$("#disptxt").addClass("opst2");
		$("#btndown").removeClass("downoff");
		$("#btndown").addClass("downon");
		document.getElementById("allow").style.display= "none";
		document.getElementById("reject").style.display= ""
	}

	var i;
	var entries = new Array();
	var every = new Array();
	//var all_str = "www.baidu.com;www.sina.com.cn;";
	var all_str = "<% getHeInfo("getURLfilterRules"); %>";

	if(all_str.length){
		entries = all_str.split(";");
		for(i=0; i < (entries.length-1) && entries[0] != ""; i++){
			every = entries[i].split(",");
			/*every[1] is current mode , every[2] is entry mode*/
			if(every[1] == every[2]){
			DelAllList.push(i)
			document.getElementById("newap_"+i).value = every[0];
			document.getElementById("em_"+i).style.display = "";
			}
		}
	}
}

function Onload()
{
	parseAllData();
}

function sortNum(a,b) 
{ 
    return (a-b); 
} 
function afterSubmit(){
  parent.TINY.box.hide();
  parent.location.reload();  
}
function submitURL()
{
	var listNum = new Array();

	var Enable = document.getElementById("apselect_0").value;

	if(document.getElementById("apselect_0").value == "0" )
	{
//		makeRequest("/cgi-bin/firewall.cgi", "firewall=HeMu_URLFilterDelete&" + DelAllList.join(";") + "&apselect_0=" + Enable, null);
		DelAllList.splice(0, DelAllList.length);
	}
	else
	{
		if (Enable == 1)
			document.MACBasicSettings.action.value = "Accept";
		else if (Enable == 2)
			document.MACBasicSettings.action.value = "Drop";

		
		if (DelControlList.length > 0)
		{
			listNum = DelControlList.sort(sortNum);
//			makeRequest("/cgi-bin/firewall.cgi", "firewall=HeMu_URLFilterDelete&" + listNum.join(";") + "&apselect_0=" + Enable, null);
			DelControlList.splice(0, DelControlList.length);
		}
				
	}

	document.MACBasicSettings.submit();
	//alert("正在设置请稍等.....");
	Alert("正在保存设置，请稍候", 5);
	setTimeout("Alert('设置成功', 3)", 5000);
	setTimeout('parseAllData();location.reload();', 3000);
}
function deleteAccessPolicyListHandler()
{
	if (http_request.readyState == 4) {
		if (http_request.status == 200) {
				window.location.reload(false);
		} else {
			//alert('There was a problem with the request.'+http_request.readyState+':'+http_request.status);
		}
	}	
}

function del(iid)
{
	//makeRequest("/goform/APDeleteAccessPolicyList", "0,"+iid, deleteAccessPolicyListHandler);	
	document.getElementById("em_"+iid).style.visibility = "hidden";
	document.getElementById("em_"+iid).style.display = "none";
	document.getElementById("newap_"+iid).value = "disabled";
	DelControlList.push(iid);
}

//防止缓存
//window.setInterval("refreshCookie()", 1000);  

</script>
</head>

<body onLoad="Onload()">
<div class="int">
	<h1>防火墙管理</h1>
    <div class="cfg">
        <div class="cfgleft">
            <ul>
				<li><a href="ALGconfig.asp">ALG</a></li>
                <li><a href="IPfilter.asp">IP过滤</a></li>
				<li class="on"><a href="URLfilter.asp">网址过滤</a></li>
				<li><a href="MACfilter.asp">MAC过滤</a></li>
            </ul>
        </div>
        <form method="post" name="MACBasicSettings" id="MACBasicSettings" action="/boafrm/heUIURLfilter">
		<input type=hidden name=firewall value=HeMu_URLfilter>
		<input type=hidden name=wlan_conf value="2860">
        <input type="hidden" id="apselect_0" name="apselect_0" >
        <!-- <input type="hidden" id="reloadweb" name="reloadweb" value="reloadwebture"> -->
        <div class="cfgright" style="padding-top:10px;padding-left: 12px;">
            <table width="100%" border="0" cellspacing="0" cellpadding="0">
              <tr>
                <td width="8%" align="left">开关</td>
                <td width="92%">
									<div class="selectswitch" id="state">
										<em class="closestate"></em><span><strong id="switch"></strong></span><em class="openstate"></em>
									</div>
								</td>
				<td style="display: none"><input type="text" name="action" value="Drop"></td>
              </tr>
            </table>
			<div class="swindow" id="swindow" style="display: ">
				<div class="sleft">
					<div class="fixblock">
						<div class="up" id="btnup"></div>
						<div class="op" id="disptxt">
							<p>白名单设置</p>
							<p>黑名单设置</p>
						</div>
						<div class="down" id="btndown"></div>
					</div>
				</div>
				<div class="sright">
					<div class="statedisp">
						<div id="allow" style="display: ">
							<strong>当前设置为允许以下URL地址访问</strong><span onClick="addnewURL();">新增URL地址</span>
						</div>
						<div id="reject" style="display: none">
							<b>当前设置为不允许以下URL地址访问</b><span onClick="addnewURL();">新增URL地址</span>
						</div>
					</div>
					<div class="maclist">
						<ul>
						<script language="JavaScript" type="text/javascript">	
							var i ;
							for (i = 0; i <= 32; i++)
							{
							
								document.write("<li id=\"em_"+i+"\" style=\"display: none\"><em><input name=\"select_"+i+"\" value=\"enabled\" id=\"newap_"+i+"\" size=16 maxlength=20 bgcolor=\"#e1e1e1\" readonly ></em><span onclick=\"del("+i+");\"></span></li>")	;
							}
						</script>		
						<li id="MACadd" name="MACadd"	style="display:none"><em><input type="text" id="addURLFilter" name="addURLFilter" size=16 maxlength=32 > </em></li>	
						</ul>
					</div>
				</div>
			</div>
			<div class="macinfo" id="macinfo" style="display: none"></div>
            <div class="subpart">
                <ul>
                    <li ><b onClick="javascript:submitURL();">保存</b></li>
                    <li><b class="cancel" onClick="javascript:parent.TINY.box.hide();">取消</b></li>
                </ul>
            </div>
			<input type="hidden" value="/herouter/URLfilter.asp" name="submit-url">

        </div>
      </form>
    </div>
</div>
<script type="text/javascript">
	//document.MACBasicSettings.apselect_0.value = AccessPolicy
$(document).ready(function(){	
	$("#switch").click(function(){
		if(document.MACBasicSettings.apselect_0.value != 0){
			 $("#state").addClass("switchoff");
			 $(".swindow").css("display","none");
			 $(".macinfo").css("display","none");
			 document.MACBasicSettings.apselect_0.value = 0;
		}else{
			var enable;
			if ( <% getHeInfo("currentURLfiltermode"); %> ){
			    enable = 1;
		    }
		    else
		        enable = 2;
			document.MACBasicSettings.apselect_0.value = enable;
			if(document.MACBasicSettings.apselect_0.value == 1){
			    $("#state").removeClass("switchoff");
		        $(".swindow").css("display","");
			    //$(".macinfo").css("display","");
		    	$("#btndown").removeClass("downon");
	    		$("#btndown").addClass("downoff");
	    		$("#disptxt").removeClass("opst2");
    			$("#btnup").removeClass("upoff");
	    		$("#btnon").addClass("upon");
	        	document.getElementById("allow").style.display= "";
        		document.getElementById("reject").style.display= "none"
        	}else{
				$("#state").removeClass("switchoff");
				$(".swindow").css("display","");
				//$(".macinfo").css("display","");
				$("#btnup").removeClass("upon");
				$("#btnup").addClass("upoff");
				$("#disptxt").addClass("opst2");
				$("#btndown").removeClass("downoff");
				$("#btndown").addClass("downon");
			
				document.getElementById("allow").style.display= "none";
				document.getElementById("reject").style.display= "";
			}
		}
	});	
	$("#btnup").click(function(){
		if(document.MACBasicSettings.apselect_0.value == 1){
			$("#btnup").removeClass("upon");
			$("#btnup").addClass("upoff");
			$("#disptxt").addClass("opst2");
			$("#btndown").removeClass("downoff");
			$("#btndown").addClass("downon");
			document.MACBasicSettings.apselect_0.value = 2;
			document.getElementById("allow").style.display= "none";
			document.getElementById("reject").style.display= "";

			var i;
			var entries = new Array();
			var every = new Array();
			//var all_str = "www.baidu.com;www.sina.com.cn;";
			var all_str = "<% getHeInfo("getURLfilterRules"); %>";

			if(all_str.length){
				entries = all_str.split(";");
				for(i=0; i < (entries.length-1) && entries[0] != ""; i++){
					every = entries[i].split(",");
					/*every[1] is current mode , every[2] is entry mode*/
					if(every[2] == "0"){
						DelAllList.push(i)
						document.getElementById("newap_"+i).value = every[0];
						document.getElementById("em_"+i).style.visibility = "";
						document.getElementById("em_"+i).style.display = "";
					}
					else
					{	
						document.getElementById("em_"+i).style.visibility = "hidden";
						document.getElementById("em_"+i).style.display = "none";
					}
				}
			}
		}
	});
	$("#btndown").click(function(){
		if(document.MACBasicSettings.apselect_0.value == 2){
			$("#btndown").removeClass("downon");
			$("#btndown").addClass("downoff");
			$("#disptxt").removeClass("opst2");
			$("#btnup").removeClass("upoff");
			$("#btnon").addClass("upon");
			document.MACBasicSettings.apselect_0.value = 1 ;
			document.getElementById("allow").style.display= "";
			document.getElementById("reject").style.display= "none";

			var i;
			var entries = new Array();
			var every = new Array();
			//var all_str = "www.baidu.com;www.sina.com.cn;";
			var all_str = "<% getHeInfo("getURLfilterRules"); %>";

			if(all_str.length){
				entries = all_str.split(";");
				for(i=0; i < (entries.length-1) && entries[0] != ""; i++){
					every = entries[i].split(",");
					/*every[1] is current mode , every[2] is entry mode*/
					if(every[2] == "1"){
						DelAllList.push(i)
						document.getElementById("newap_"+i).value = every[0];
						document.getElementById("em_"+i).style.visibility = "";
						document.getElementById("em_"+i).style.display = "";
					}
					else
					{	
						document.getElementById("em_"+i).style.visibility = "hidden";
						document.getElementById("em_"+i).style.display = "none";
					}
				}
			}
		}
	});
});
</script>
</body>
</html>
