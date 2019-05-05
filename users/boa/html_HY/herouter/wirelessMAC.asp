<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>互联网设置</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="js/herouter.js"></script>
<style type="text/css">
<!--
body{ color:#676767;}
*{-moz-transition: all 0.3s ease-in;-webkit-transition: all 0.3s ease-in;-o-transition: all 0.3s ease-in;transition: all 0.3s ease-in;}
*:hover{-moz-transition: all 0.3s ease-in;-webkit-transition: all 0.3s ease-in;-o-transition: all 0.3s ease-in;transition: all 0.3s ease-in;}
.int { height:410px; width:670px; overflow:hidden; background:#fff url(images/popbg.gif) repeat-x 0 -2px; padding:5px 15px;}
.int h1{ font-size:18px; line-height:50px; }
.int h1 span{ font-size:12px; color:#919191; padding-left:15px; font-weight:normal;}
.mainsetting{ padding:30px; }
.mainsetting h2{ font-size:14px; line-height:38px;}
.c1{ height:48px;}
.c1 b{ display:block; height:48px; cursor:pointer; width:190px; font-weight:normal; float:left; background:#77d1df; font-size:18px; color:#fff; text-align:center; line-height:48px;}
.c1 a{ background:url(images/help.png) no-repeat center center; display:block; float:left; height:48px; width:30px;}
.c1 b:hover{ background:#56bbcb;}
.c1 span{  font-size:12px; color:#919191; padding:0 0 0 15px; float:left; font-weight:normal; line-height:48px;}
.c1 .err{ background:#e71b1b;}
.c1 .err:hover{ background:#da1515;}
.mainsetting h3{ font-size:24px; font-weight:normal; line-height:50px; color:#29b8e4;}
.c2{ border-top:1px #e6e6e6 solid; margin-top:80px; line-height:30px;}

.cfg{ padding:15px 0; height:330px;}
.cfgleft{ float:left; width:185px; height:330px; background:url(images/cfgleftbg.png) repeat-y;}
.cfgleft ul{}
.cfgleft li{ cursor:pointer; height:40px; line-height:40px; font-size:14px; color:#666; border-left:4px #dfdfdf solid ; padding-left:12px;}
.cfgleft li:hover{ border-left:4px #1e9ec5 solid;  font-weight:bold;}
.cfgleft li a,.cfgleft li a:visited{color:#666; display:block; height:40px;width:170px; }
.cfgleft li a:hover,.cfgleft li a:active{ text-decoration:none;}
.cfgleft li.on{ border-left:4px #1e9ec5 solid; color:#1e9ec5; font-weight:bold; font-size:14px; background:#fff url(images/arrow1.png) no-repeat 170px center;}
.cfgleft li.on a{color:#1e9ec5;}
.cfgright{ float:right; width:470px; height:330px;}
.cfgright td{ height:36px; font-size:14px;}
.cfgright td span{ padding-left:50px; font-size:14px;}
.cfgright .spheight{ height:60px; }
.cfgright td label{ font-size:14px; line-height:60px;}
.radiobox{ margin-top:-2px;}
.inp1 {height:26px; border:1px #d2d2d2 solid; width:160px;font-family:"微软雅黑"; color:#666; padding:0 10px;}
.subpart{ height:48px; padding-left:120px; padding-top:25px;}
.subpart ul{}
.subpart li{ float:left; padding-right:10px;}
.subpart b{ display:block; height:48px; cursor:pointer; width:170px; font-weight:normal; float:left; background:#77d1df; font-size:18px; color:#fff; text-align:center; line-height:48px;}
.subpart b:hover{ background:#56bbcb;}
.subpart .cancel{ width:110px; background:#99e4f0; }

.selectswitch{ height:29px;}
.selectswitch em{ float:left; height:20px; width:20px; display:block; margin:4px; padding:0;}
.selectswitch .closestate{ background:url(images/wifistateicon.png) no-repeat;}
.selectswitch .openstate{background:url(images/wifistateicon.png) no-repeat 0 -40px;}
.cfgright td .selectswitch span{ float:left; padding-left:0px; background:url(images/selbg.png) no-repeat; width:55px; height:29px; display:block;}
.selectswitch strong{ display:block; margin:3px; height:23px; width:49px; background:url(images/select.png) no-repeat right center; cursor:pointer;}
.switchoff{}
.switchoff .closestate{ background-position:0 -20px;}
.switchoff .openstate{ background-position:0 -60px;}
.switchoff  strong{ background-position:left center;}




.swindow{ height:165px; margin-top:5px; border-left:1px #f0f0f0 dashed; border-right:1px #f0f0f0 dashed; border-top:1px #f0f0f0 dashed; ;}
.sleft{ float:left; margin:8px; width:100px; height:149px; overflow:hidden;background:url(images/macarrowbg.png) repeat-x 0 center;}
.fix{ position:absolute; height:149px; overflow:hidden; width:100px; }
.sleft .up{ height:40px; cursor:pointer; background: url(images/macarrow.png) no-repeat center 0; position:absolute; width:100px; z-index:999;}
.sleft .down{ height:40px; cursor:pointer; width:100px;  background: url(images/macarrow.png) no-repeat center -120px;  position:absolute; bottom:0; z-index:998;}
.sleft .op{ height:298px; top:0; width:100px; position:absolute; z-index:99; text-align:center;}
.sleft .op p{ height:93px; display:block; vertical-align:middle; padding-top:56px; font-size:14px;}
.sleft .opst2{ top:-149px;}
.macinfo{ height:28px; background:#f2f2f2; padding:0 5px; line-height:28px; color:#8b8b8b;}
.sleft .upon{ background-position: center 0;}
.sleft .upoff{ background-position: center -40px;}
.sleft .downon{ background-position: center -80px;}
.sleft .downoff{ background-position:center -120px;}
.sright{ height:149px; margin:8px 0; width:350px; float:right; border-left:1px #beeaf7 solid;}
.statedisp{ height:28px; padding:0 8px 5px 8px;line-height:28px;}
.statedisp strong{ color:#2d9319; font-size:12px;  font-weight:normal; float:left;}
.statedisp span{ float:right; display:block; height:28px; width:100px; font-size:14px; text-align:center; color:#fff; background:#77d1df; cursor:pointer; }
.statedisp span:hover{ background:#56bbcb;}
.maclist{ height:115px; overflow-x:hidden; overflow-y:auto; padding:0 0 0 5px;}
.maclist ul{}
.maclist li{ float:left; width:152px; height:28px; border:1px #d1d1d1 dashed; margin:5px;}
.maclist li input{ border:none; height:27px; width:135px; line-height:28px; padding:0 5px; font-family:Arial,"microsoft yahei", sans-serif;}
.maclist li em{ font-style:normal; padding:0 5px; line-height:28px; float:left; width:112px; display:block;}
.maclist li span{ display:block; width:28px; float:right; height:20px; margin:4px 0; border-left:1px #e1e1e1 solid; background:url(images/delmac.png) no-repeat center center; cursor:pointer;}

.selectinp{height:28px; border:1px #d2d2d2 solid; width:230px; line-height:26px; font-family:"微软雅黑";color:#666;padding:0 10px; background:#fff url(images/dropd.png) no-repeat right center;}
.fixchanstyle{ position:absolute;}
.bm {margin-top:30px;position:absolute;}
.ganyin{ position:absolute; height:30px;cursor:pointer; border:1px #333 solid; z-index:100001; background:#0099CC;filter:alpha(opacity=0);-moz-opacity:0;-khtml-opacity: 0;opacity: 0;}
.bm ul{ padding:1px 0 0 0; border-bottom:1px #eee solid; border-left:1px #eee solid; border-right:1px #eee solid; }
.bm li{ color:#0781d9; line-height:30px; padding-left:10px; height:30px; background:#fff; font-size:12px; text-align:left; padding-right:12px; display:block;}
.bm .nowon{ color:#adadae;}
.bm li:hover{ background-color:#b2e9fd;cursor:pointer;}
-->
</style>
<script type="text/javascript">
var AccessControlList = new Array();
var AccessPolicy = 0;
var http_request = false;

var DelControlList = new Array();
var cfg_changed = 0;
var switch_changed = 0;
var btn_changed = 0;
var ssid_cur = 0;
var wlan_num = <% getIndex("wlan_num"); %>;
var is_first_time=1;
function $$$$$(_sId){
 return document.getElementById(_sId);
 }
function hide(_sId){
	$$$$$(_sId).style.display = $$$$$(_sId).style.display == "none" ? "" : "none";
 }

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
	cfg_changed = 1;
	document.getElementById("MACadd").style.display = "";
}

function parseAllData(str)
{
	var all_str = new Array();
	all_str = str.split("&");
	var i;
	
	AccessControlList = all_str[1].split(";");
	AccessPolicy = all_str[2];
	document.MACBasicSettings.apselect_0.value = AccessPolicy;
	document.MACBasicSettings.macInfoValue.value = all_str[3];

	if(cfg_changed == 0) {
		for(i = 0; i<=32; i++)
		{
			document.getElementById("newap_"+i).value = 0;
			document.getElementById("em_"+i).style.visibility = "";
			document.getElementById("em_"+i).style.display = "none";
		}
	}
	
	for(i = 0; i<= 32 && AccessControlList[0] != "" && (typeof(AccessControlList[i])!="undefined");i++)
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
	if(is_first_time)
	{
		document.getElementById("bandselect").value="2.4G";
		document.getElementById("ssidIndex").value=0;
	}
	if(wlan_num == 1)
	{
		document.getElementById("band_table").style.visibility = "hidden";
		document.getElementById("band_table").style.display = "none"; 		
	}
	makeRequest("/boafrm/wirelessGetMAClist", "ssidIndex="+document.getElementById("ssidIndex").value, MAClistHandler);
	AutoChoose();
}

function sortNum(a,b) 
{ 
    return (b-a); 
} 

function reboot_internet() {
	$.ajax({
      	type: "post",
        url: "/boafrm/doInitInternet",         
        success: function(data){
        },
        error: function(data) {
        }
    })
}

function submitmac()
{
	var listNum = new Array();

	if (DelControlList.length <= 0 && document.getElementById("newap_text_0").value == "" && 
		document.getElementById("apselect_0").value != 0 )
	{
		alert("请增加或删除MAC地址!\n");
		return;		
	}
	
	if (DelControlList.length > 0)
	{
		listNum = DelControlList.sort(sortNum);
		makeRequest("/boafrm/APBatchDeleteAccessPolicyList", listNum.join(";"), null);
		DelControlList.splice(0, DelControlList.length);
	}
	
	if( document.getElementById("newap_text_0").value != "")
	{
		if(!checkMac(document.getElementById("newap_text_0").value))
		{
			alert(" 无效的MAC地址!\n");
			return;
		}
		if ((document.getElementById("newap_text_0").value == "E4:3F:A2:03:03:32") || 
				(document.getElementById("newap_text_0").value == "E4:3F:A2:03:03:31")) 
		{
			alert("MAC地址不合法!\n");
			return;
		}
	}
	//document.MACBasicSettings.submit();
	$.ajax({
		type: "post",
		url: "/boafrm/APSecurity",     
		data: $("#MACBasicSettings").serialize(),    
		success: function(data){
		},
		error: function(data) {
		}
	})

	//alert("正在设置请稍等.....");
	cfg_changed = 0;
	btn_changed = 0;
	switch_changed = 0;
	Alert("正在保存设置，请稍候", 10);

	setTimeout("reboot_internet();Alert('设置成功，无线网络将断开，请在30秒后重新进入本页面', 30)", 10100);
	setTimeout('makeRequest("/boafrm/wirelessGetMAClist", "ssidIndex="+document.getElementById("ssidIndex").value, MAClistHandler);location.reload();', 40200);
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
	cfg_changed = 1;
}

/*Display the element*/
function AutoChoose(){
	if(document.MACBasicSettings.apselect_0.value == 0){
		$("#state").addClass("switchoff");
		$(".swindow").css("display","none");
		$(".macinfo").css("display","none");
	}	
	else if(document.MACBasicSettings.apselect_0.value == 1){
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
	else if(document.MACBasicSettings.apselect_0.value == 2){
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
	};
}
$(document).ready(function(){

	$("#switch").click(function(){	
		if(switch_changed != 0)
			switch_changed = 0;
		else
			switch_changed = 1;
			
		if(document.MACBasicSettings.apselect_0.value != 0){
			document.MACBasicSettings.apselect_0.value = 0;
		} else {		
			if(AccessPolicy != 0) /*restore apselect_0, when switch even number*/
				document.MACBasicSettings.apselect_0.value = AccessPolicy;
			else /*set default value 1*/
				document.MACBasicSettings.apselect_0.value = 1;			
		}
		
		AutoChoose();
	});	
	
	$("#btnup").click(function(){
		if(btn_changed)
			btn_changed = 0;
		else
			btn_changed = 1;
			
		if(document.MACBasicSettings.apselect_0.value == 1){
			document.MACBasicSettings.apselect_0.value = 2;
			AccessPolicy = document.MACBasicSettings.apselect_0.value;/*sync modify*/
			AutoChoose();
		}
	});
	$("#btndown").click(function(){
		if(btn_changed)
			btn_changed = 0;
		else
			btn_changed = 1;

		if(document.MACBasicSettings.apselect_0.value == 2){
			document.MACBasicSettings.apselect_0.value = 1;
			AccessPolicy = document.MACBasicSettings.apselect_0.value;/*sync modify*/
			AutoChoose();
		}
	});
});

function wlan_band_select(value)
{
	if(value=="2.4G")
	{
		document.getElementById("bandselect").value=value;
		document.getElementById("ssidIndex").value=0;
		hide("chanstyle");
	}
	if(value=="5G")
	{
		document.getElementById("bandselect").value=value;
		document.getElementById("ssidIndex").value=1;
		hide("chanstyle");		
	}
	ssidChange();
}
function ssidChange() 
{
	var confirm_log;
	
	if(ssid_cur != document.getElementById("ssidIndex").value &&(cfg_changed||switch_changed||btn_changed)) {
		if(ssid_cur == 0)
			confirm_log = "当前修改还未提交，确认切换到5G?"
		else
			confirm_log = "当前修改还未提交，确认切换到2.4G?"
			
		if(confirm(confirm_log)==true) {
			ssid_cur = document.getElementById("ssidIndex").value;
			
			/*restore the modify*/
			document.getElementById("MACadd").style.display = "none";
			document.getElementById("newap_text_0").value="";
			DelControlList.splice(0, DelControlList.length);
			cfg_changed = 0;
			switch_changed = 0;
			btn_changed = 0;
		} else {
			document.getElementById("ssidIndex").value = ssid_cur;
			return;
		}
	} else {
		ssid_cur = document.getElementById("ssidIndex").value;
	}
	is_first_time=0;
	Onload();
}

</script>
</head>

<body onload="Onload()">
<div class="int">
	<h1>高级设置</h1>
    <div class="cfg">
        <div class="cfgleft">
            <ul>
                <li><a href="entconfig.asp">局域网设置</a></li>
                <li><a href="DHCPbanding.asp">DHCP设置</a></li>
				<li><a href="MACClone.asp">MAC地址克隆</a></li>
				<li><a href="wirelesschannel.asp">无线信道设置</a></li>
				<li class="on"><a href="wirelessMAC.asp">无线MAC地址访问控制</a></li>
				<li><a href="systemtime.asp">系统时间管理</a></li>
				<li><a href="sysupdate.asp">系统升级</a></li>
				<li><a href="restoredefault.asp">恢复出厂设置</a></li>
            </ul>
        </div>
        <form method="post" name="MACBasicSettings" id="MACBasicSettings" action="/boafrm/APSecurity">
        <input type="hidden" id="apselect_0" name="apselect_0">
        <input type="hidden" id="reloadweb" name="reloadweb" value="reloadwebture">
        <div class="cfgright" style="padding-top:10px;">
			<table id="band_table" width="100%" border="0" cellspacing="0" cellpadding="0">
<!--
				<tr>
	              <td width="14%" align="right">频段:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </td>
		              <td width="86%">
			              <select id="ssidIndex" name="ssidIndex" onchange="ssidChange()">
			                <option value=0>2.4G</option>
			                <option value=1>5G</option>
			              </select>
		              </td>
	            </tr>
	            -->
	             <tr id="band_select">
					  <td width="8%" align="right" id="bandtd" style="font-size:14px">频段</td>
							<td style="padding-left:10px"align="left">
								<div class="fixchanstyle" style=" z-index:999;">
									<div class="ganyin" onclick="hide('chanstyle')" style=" width:250px;"></div>
										<div id="chanstyle" style="display: none; " class="bm">
										<ul style=" width:250px;">
											<li  onclick="wlan_band_select('2.4G')" >2.4G</li>
											<li  onclick="wlan_band_select('5G')">5G</li>
										</ul>
									</div>
								</div>
							<input type="text" class="selectinp" id="bandselect" value="" />
							<input type="hidden" id="ssidIndex" name="ssidIndex" value="">
							</td>
		  			</tr>
			</table>
            <table width="100%" border="0" cellspacing="0" cellpadding="0">
              <tr>
                <td width="8%" align="right">开关</td>
                <td width="92%">
					<div class="selectswitch" id="state" class="openstate">
						<em class="closestate"></em><span><strong id="switch"></strong></span><em class="openstate"></em>
					</div>
				</td>
              </tr>
            </table>
			<div class="swindow" id="swindow" style="display: ">
				<div class="sleft">
					<div class="fix">
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
							<strong>当前设置为仅允许以下MAC地址访问</strong><span onclick="addnewMAC();">新增MAC地址</span>
						</div>
						<div id="reject" style="display: none">
							<strong>当前设置为不允许以下MAC地址访问</strong><span onclick="addnewMAC();">新增MAC地址</span>
						</div>
					</div>
					<div class="maclist">
						<ul>
						<script language="JavaScript" type="text/javascript">	
						var i ;
						for (i = 0; i <= 32; i++)
						{
							document.write("<li id=\"em_"+i+"\" style=\"display: none\"><em><input id=\"newap_"+i+"\" size=16 maxlength=20 bgcolor=\"#e1e1e1\" readonly ></em><span onclick=\"del("+i+");\"></span></li>")	;
						}							
						</script>	
						<li id="MACadd" name="MACadd"style="display:none"><em><input type="text" id="newap_text_0" name="newap_text_0" size=16 maxlength=20> </em></li>			
						</ul>
					</div>
				</div>
			</div>
			<div class="macinfo" id="macinfo" style="display: ">当前使用设备 MAC 地址：
				<input type="text" id="macInfoValue" name="macInfoValue" size=16 maxlength=20 style="border:none;" readonly>
			</div>
            <div class="subpart">
                <ul>
				<li><b onclick="javascript:submitmac();">保存</b></li>
				<li><b class="cancel" onClick="javascript:parent.TINY.box.hide();">取消</b></li>
                </ul>
            </div>
        </div>
      </form>
    </div>
</div>
