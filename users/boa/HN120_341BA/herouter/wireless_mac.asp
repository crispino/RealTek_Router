<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" /> 
<title>无线访问控制一</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="js/herouter.js"></script>
<script type="text/javascript">
var AccessControlList = new Array();
var AccessPolicy = 0;
var http_request = false;
var controlNum = '<%getHeInfo("wlanMaxMacACNum")%>';
var DelControlList = new Array();
var wlan_num = '<%getHeInfo("wlan_num")%>';

function checkMac(str){
	var len = str.length;
	if(len!=17)
		return false;

	for (var i=0; i<str.length; i++) {
		if((i%3) == 2){
			if(str.charAt(i) == ':')
				continue;
		}else{
			if (    (str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
					(str.charAt(i) >= 'a' && str.charAt(i) <= 'f') ||
					(str.charAt(i) >= 'A' && str.charAt(i) <= 'F') )
			continue;
		}
		return false;
	}
	return true;
}
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

function addnewMAC()
{
	document.getElementById("MACadd").style.display = "";
}
function parseAllData(str)
{	
	var all_str = new Array();
	all_str = str.split("&");
	var i;
	
	AccessControlList = all_str[1].split(";");
	AccessPolicy = all_str[2];
	currentMac = all_str[3].toUpperCase();
	document.MACBasicSettings.apselect_0.value = AccessPolicy;
	if(currentMac)
		document.getElementById('macinfo').innerHTML='当前使用设备MAC地址：'+currentMac; // DUT MAC
	
	var switchcode=0;
	if(document.MACBasicSettings.apselect_0.value == 0){
		$("#state").addClass("switchoff");
		$(".swindow").css("display","none");
		$(".macinfo").css("display","none");
	}	
	else if(document.MACBasicSettings.apselect_0.value == 1){ // allow
		$("#state").removeClass("switchoff");
		$(".swindow").css("display","");
		$(".macinfo").css("display","");
		$("#btndown").removeClass("downon");
		$("#btndown").addClass("downoff");
		$("#disptxt").removeClass("opst2");
		$("#btnup").removeClass("upoff");
		$("#btnon").addClass("upon");
	  document.getElementById("allow").style.display= "";
	  document.getElementById("reject").style.display= "none"
	}	
	else if(document.MACBasicSettings.apselect_0.value == 2){ // deny
		$("#state").removeClass("switchoff");
		$(".swindow").css("display","");
		$(".macinfo").css("display","");
		$("#btnup").removeClass("upon");
		$("#btnup").addClass("upoff");
		$("#disptxt").addClass("opst2");
		$("#btndown").removeClass("downoff");
		$("#btndown").addClass("downon");
		document.getElementById("allow").style.display= "none";
		document.getElementById("reject").style.display= ""
	}
	
	for(i = 0; i<= controlNum && AccessControlList[0] != "" ;i++)
	{
		if(i < AccessControlList.length)
		{
			document.getElementById("newap_"+i).value = AccessControlList[i];
			document.getElementById("em_"+i).style.display = "";
		}
		else
		{	
			document.getElementById("em_"+i).style.visibility = "hidden";
			document.getElementById("em_"+i).style.display = "none";
		}
	}
}


function afterSubmit() {
  parent.TINY.box.hide();
  parent.location.reload();
}

function MAClistHandler()
{
	if (http_request.readyState == 4) {
		if (http_request.status == 200) {
			parseAllData(http_request.responseText);
		} else {			
			//alert('There was a problem with the request.'+http_request.readyState+':'+http_request.status);
		}
	}	
}
function Onload()
{
	makeRequest("/boafrm/wirelessGetMAClist", "ssidIndex=0", MAClistHandler);
}

function sortNum(a,b) 
{ 
    return (b-a); 
} 

function reboot_internet() {
		    $.ajax({
      	type: "get",
        url: "/goform/doInitInternet",         
        success: function(data){
        },
        error: function(data) {
        }
      })
}

function submitmac()
{
	var listNum = new Array();

	/* disable mac control */
	if(document.getElementById("apselect_0").value == "0" )
	{		
		makeRequest("/boafrm/APSecurity", "apselect_0=0&phyband=2", null);  
	}
	else
	{
		if (DelControlList.length <= 0 && document.getElementById("newap_text_0").value == "")
		{
				alert("请增加或删除MAC地址!\n");
				return;		
		}
		
		if (DelControlList.length > 0)
		{
			listNum = DelControlList.sort(sortNum);
			if( document.getElementById("newap_text_0").value != "")
				makeRequest("/boafrm/APBatchDeleteAccessPolicyList", listNum.join(";")+"#ssidIndex=0#addMac=1#", null);
			else
				makeRequest("/boafrm/APBatchDeleteAccessPolicyList", listNum.join(";")+"#ssidIndex=0#macEnable="+document.getElementById("apselect_0").value, null);
			DelControlList.splice(0, DelControlList.length);
		}
		
		if( document.getElementById("newap_text_0").value != "")
		{
			if(!checkMac(document.getElementById("newap_text_0").value))
			{
				alert(" 无效的MAC地址!\n");
				return;
			}
			if ((document.getElementById("newap_text_0").value == "E4:3F:A2:03:16:7A") || 
					(document.getElementById("newap_text_0").value == "E4:3F:A2:03:16:79")) 
			{
				alert("MAC地址不合法!\n");
				return;
			}
			
			//document.MACBasicSettings.submit();
			  $.ajax({
			type: "post",
			url:  "/boafrm/APSecurity",     
			data: $("#MACBasicSettings").serialize(),    
			success: function(data){
			},
			error: function(data) {
			}
		  })
		}
	}
	
	//alert("正在设置请稍等.....");
	Alert("正在保存设置，请稍候", 10);
	setTimeout("Alert('设置成功，无线网络将断开，请在30秒后重新进入本页面', 30)", 10100);
	setTimeout('makeRequest("/boafrm/wirelessGetMAClist", "ssidIndex=0", MAClistHandler);location.reload();', 40200);
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
	DelControlList.push(iid);
}

</script>
</head>

<body onLoad="Onload()">
<div class="int">
	<h1>无线设置</h1>
    <div class="cfg">
        <div class="cfgleft">
            <ul>
                <li><a class="ifdisabled" href="wireless_guest.asp">2.4G访客网络</a></li>
				<li><a href="wireless_wps.asp">2.4GWPS设置</a></li>
				<li class="on"><a href="wireless_mac.asp">2.4G无线访问控制</a></li>
				<script language="JavaScript" type="text/javascript">
				if(wlan_num>1){
					document.write("<li><a class=\"ifdisabled\" href=\"wireless_guest_5G.asp\">5G访客网络</a></li>");
					document.write("<li><a href=\"wireless_wps_5G.asp\">5GWPS设置</a></li>");
					document.write("<li><a href=\"wireless_mac_5G.asp\">5G无线访问控制</a></li>");					
				}							
				</script>				
            </ul>
        </div>
        <form method="post" name="MACBasicSettings" id="MACBasicSettings" action="/boafrm/APSecurity">
		<input type=hidden name=page value=HeMu_maclist>
		<input type=hidden name=wlan_conf value="2860">
        <input type="hidden" id="ssidIndex" name="ssidIndex" value="0">
        <input type="hidden" id="apselect_0" name="apselect_0" >
        <input type="hidden" id="phyband" name="phyband" value="2">
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
							<strong>当前设置为仅允许以下MAC地址访问</strong><span onClick="addnewMAC();">新增MAC地址</span>
						</div>
						<div id="reject" style="display: none">
							<b>当前设置为不允许以下MAC地址访问</b><span onClick="addnewMAC();">新增MAC地址</span>
						</div>
					</div>
					<div class="maclist maclist2 ">
						<ul>
<script language="JavaScript" type="text/javascript">	
var i ;
for (i = 0; i <= controlNum; i++)
{
	document.write("<li id=\"em_"+i+"\" style=\"display: none\"><em><input id=\"newap_"+i+"\" size=16 maxlength=20 bgcolor=\"#e1e1e1\" readonly ></em><span onclick=\"del("+i+");\"></span></li>")	;
}							
</script>		
						<li id="MACadd" name="MACadd"	style="display:none"><em><input type="text" id="newap_text_0" name="newap_text_0" size=16 maxlength=20 > </em></li>			
						</ul>
					</div>
				</div>
			</div>
			<div class="macinfo" id="macinfo" ></div>
            <div class="subpart">
                <ul>
                    <li ><b onClick="javascript:submitmac();">保存</b></li>
                    <li><b class="cancel" onClick="javascript:parent.TINY.box.hide();">取消</b></li>
                </ul>
            </div>
			

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
			$("#state").removeClass("switchoff");
			$(".swindow").css("display","");
			$(".macinfo").css("display","");
			document.MACBasicSettings.apselect_0.value = 1;
			$("#btndown").removeClass("downon");
			$("#btndown").addClass("downoff");
			$("#disptxt").removeClass("opst2");
			$("#btnup").removeClass("upoff");
			$("#btnon").addClass("upon");
		  document.getElementById("allow").style.display= "";
		  document.getElementById("reject").style.display= "none"			
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
		}
	});
});
</script>
</body>
</html>
