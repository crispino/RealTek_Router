<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" /> 
<title>防火墙管理</title>
<!--<link href="css/style_main.css" rel="stylesheet" type="text/css" />-->
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="js/herouter.js"></script>
<style type="text/css">
<!--
body{ color:#676767;}
*{-moz-transition: all 0.3s ease-in;-webkit-transition: all 0.3s ease-in;-o-transition: all 0.3s ease-in;transition: all 0.3s ease-in;}
*:hover{-moz-transition: all 0.3s ease-in;-webkit-transition: all 0.3s ease-in;-o-transition: all 0.3s ease-in;transition: all 0.3s ease-in;}
.int { height:410px; width:670px; overflow:hidden; background:#fcfcfa url(images/fullbg.gif) repeat-x; padding:5px 15px;}
.int h1{ font-size:18px; line-height:50px; border-bottom:1px #d3d3d3 solid;}
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
.cfgleft li a,.cfgleft li a:visited{color:#666; display:block; height:40px;width:170px; }
.cfgleft li a:hover,.cfgleft li a:active{ text-decoration:none;}
.cfgleft li:hover{ border-left:4px #1e9ec5 solid;  font-weight:bold;}
.cfgleft li.on{ border-left:4px #1e9ec5 solid; color:#1e9ec5; font-weight:bold; font-size:14px; background:#ECEDEB url(images/arrow1.png) no-repeat 170px center;}
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




.swindow{ height:92px; margin-top:5px; border:1px #f0f0f0 dashed; overflow-x:hidden; overflow-y:auto; }
.vlan{ padding:0 10px;}
.sectionstyle{ height:45px; border-bottom:1px #efefef solid; }
.sectionstyle span{ float:left; font-size:14px; display:block; width:150px;line-height:45px;}
.sectionstyle em{ float:left; font-style:normal;line-height:45px;}
.sectionstyle b{ float:left; display:block; height:30px; padding:7px 5px 0 5px;}
.sectionstyle b input{ line-height:30px; width:60px; height:25px;border:1px #dedede solid; padding:0 5px;   font-family:Arial,"microsoft yahei", sans-serif;}

.selectinp{height:30px; border:1px #d2d2d2 solid; width:100px; line-height:26px; font-family:"微软雅黑";color:#666;padding:0 10px; background:#fff url(images/dropd.png) no-repeat right center;}
.fix{ position:absolute;}
.bm {margin-top:30px;position:absolute;}
.ganyin{ position:absolute; height:30px;cursor:pointer; border:1px #333 solid; z-index:100001; background:#0099CC;filter:alpha(opacity=0);-moz-opacity:0;-khtml-opacity: 0;opacity: 0;}
.bm ul{ padding:1px 0 0 0; border-bottom:1px #eee solid; border-left:1px #eee solid; border-right:1px #eee solid; }
.bm li{ color:#0781d9; line-height:30px; padding-left:10px; height:30px; background:#fff; font-size:12px; text-align:left; padding-right:12px; display:block;}
.bm .nowon{ color:#adadae;}
.bm li:hover{ background-color:#b2e9fd;cursor:pointer;}

.bandingcfg{ height:28px; padding:7px 0; font-size:14px; }
.bandingcfg strong{ line-height:28px; font-size:14px; float:left; padding-right:15px; font-weight:normal;}
.bandingcfg span{ float:left; height:28px; width:80px; display:block; text-align:center; line-height:28px; color:#fff; background:#77d1df; margin-right:10px; cursor:pointer;}
.bandingcfg span:hover{background:#56bbcb;}
.bandingcfg b{ float:left; font-weight:normal; height:28px; width:170px; display:block; text-align:center; line-height:28px; color:#fff; background:#99e4f0; cursor:pointer;}

.ipmaclist{}
.ipmaclist ul{}
.ipmaclist li{ float:left; width:340px; height:28px; border:1px #d1d1d1 dashed; margin:5px;}
.ipmaclist li input{ border:none; height:27px; width:132px; line-height:28px; padding:0 5px; font-family:Arial,"microsoft yahei", sans-serif;}
.ipmaclist li em{ font-style:normal; padding:0 5px; line-height:28px; float:left; width:300px; display:block;}
.ipmaclist li span{ display:block; width:28px; float:right; height:20px; margin:4px 0; border-left:1px #e1e1e1 solid; background:url(images/delmac.png) no-repeat center center; cursor:pointer;}

.devicelist{ position:absolute; z-index:99999; background:#f3f3f3;  top:-12px; left:200px;*left:15px; height:195px; padding:0 10px; width:450px;-moz-box-shadow:3px 3px 0px #dedede; -webkit-box-shadow:3px 3px 0px #dedede; box-shadow:3px 3px 0px #dedede; overflow:hidden;}
.devicelist h2{ height:36px; border-bottom:1px #eaeaea solid; margin-bottom:5px;}
.devicelist h2 b{ font-size:14px; float:left; line-height:36px; color:#666; color:#1e9ec5;}
.devicelist h2 span{ width:20px; height:36px; display:block; float:right; background:url(images/delmac.png) no-repeat center center; cursor:pointer;}
.devicelist .box{ height:153px; overflow:auto;}
.devicelist dl{ padding:6px; margin:3px 0; background:#fff; height:22px;}
.devicelist dt{ float:right; width:22px; height:22px; background:url(images/btnadd.png) no-repeat center center; cursor:pointer;}
.devicelist dd{ float:left; width:125px; font-size:12px; line-height:16px; color:#666;}
.changepage{ padding-top:7px; height:20px;}
.changepage em{ font-style:normal; float:left; color:#666; }
.changepage span{ float:right; margin-left:5px; display:block; height:20px; cursor:pointer; width:30px; background:url(images/macarrow.png) no-repeat center 0;}
.changepage .prev{ background-position: center -10px;}
.changepage .disprev{ background-position: center -50px;}
.changepage .next{ background-position: center -90px;}
.changepage .disnext{  background-position:center -130px}
-->
</style>
</head>
<body>
<form method="post" name="alg_setting" id="alg_setting" action="/boafrm/heUIformALGOnOff">
<input type=hidden name=firewall value=HeMu_ALG>
<div class="int">
	<h1>防火墙管理</h1>
    <div class="cfg">
		<div class="cfgleft">
            <ul>
				<li class="on"><a href="ALGconfig.asp">ALG</a></li>
                <li><a href="IPfilter.asp">IP过滤</a></li>
				<li><a href="URLfilter.asp">网址过滤</a></li>
				<li><a href="MACfilter.asp">MAC过滤</a></li>
            </ul>
        </div>
        <div class="cfgright" style="padding-top:10px;">
            <table width="100%" border="0" cellspacing="0" cellpadding="0">
              <tr>
                <td width="30%" align="left">FTP ALG</td>
                <td width="70%">
					<div class="selectswitch" id="state1">
						<em class="closestate"></em><span><strong id="switch1"></strong></span><em class="openstate"></em>
						<input type="hidden" id="ftpEn" name="ftpEn" value="">
					</div>
				</td>
              </tr>
              <tr>
              <td width="30%" align="left">SIP ALG</td>
                <td width="70%">
					<div class="selectswitch" id="state2">
						<em class="closestate"></em><span><strong id="switch2"></strong></span><em class="openstate"></em>
						<input type="hidden" id="sipEn" name="sipEn" value="">
					</div>
				</td>
              </tr>
              <tr>
                <td width="30%" align="left">RTSP ALG</td>
                <td width="70%">
					<div class="selectswitch" id="state3">
						<em class="closestate"></em><span><strong id="switch3"></strong></span><em class="openstate"></em>
						<input type="hidden" id="rtspEn" name="rtspEn" value="">
					</div>
				</td>
              </tr>
			  <tr>
                <td width="30%" align="left">L2TP ALG</td>
                <td width="70%">
					<div class="selectswitch" id="state4">
						<em class="closestate"></em><span><strong id="switch4"></strong></span><em class="openstate"></em>
						<input type="hidden" id="l2tpEn" name="l2tpEn" value="">
					</div>
				</td>
              </tr>
            </table>
			<div class="subpart">
                <ul>
                    <li><b id="subbtn">保存</b></li>
                    <li><b class="cancel" onClick="javascript:parent.TINY.box.hide();">取消</b></li>
                </ul>
            </div>
            <input type="hidden" value="/herouter/ALGconfig.asp" name="submit-url">
        </div>
    </div>
</div>
</form>
<script type="text/javascript">
	function $$$$$(_sId)
	{
 		return document.getElementById(_sId);
 	}
	function hide(_sId)
	{
		$$$$$(_sId).style.display = $$$$$(_sId).style.display == "none" ? "" : "none";
	 }
	function pick(v,targetid,abc) 
	{
		document.getElementById(targetid).value=v;
		hide(abc);
	}
	$(document).ready(function()
	{
	  var switchcode_ftp = <% getHeInfo("ftp_algonoff"); %>;
	  var switchcode_sip = <% getHeInfo("sip_algonoff"); %>;
	  var switchcode_rtsp = <% getHeInfo("rtsp_algonoff"); %>;
	  var switchcode_l2tp = <% getHeInfo("l2tp_algonoff"); %>;
	  if(switchcode_ftp == "")
		switchcode_ftp=0
	  if(switchcode_sip == "")
		switchcode_sip=0
	  if(switchcode_rtsp == "")
		switchcode_rtsp=0
	  if(switchcode_l2tp == "")
		switchcode_l2tp=0
		
	  document.alg_setting.ftpEn.value = switchcode_ftp;
	  document.alg_setting.sipEn.value = switchcode_sip;
	  document.alg_setting.rtspEn.value = switchcode_rtsp;
	  document.alg_setting.l2tpEn.value = switchcode_l2tp;
		if(switchcode_ftp==0)
		{
			$("#state1").addClass("switchoff");
			$(".swindow").css("display","none");
		}
		else
		{
			$("#state1").removeClass("switchoff");
			$(".swindow").css("display","");
		}
		if(switchcode_sip==0)
		{
			$("#state2").addClass("switchoff");
		}
		else
		{
			$("#state2").removeClass("switchoff");
		}
		if(switchcode_rtsp==0)
		{
			$("#state3").addClass("switchoff");
		}
		else
		{
			$("#state3").removeClass("switchoff");
		}
		if(switchcode_l2tp==0)
		{
			$("#state4").addClass("switchoff");
		}
		else
		{
			$("#state4").removeClass("switchoff");
		}
		$("#switch1").click(function(){
			if(switchcode_ftp==1){
				 $("#state1").addClass("switchoff");
				 switchcode_ftp=0;
				 document.alg_setting.ftpEn.value =switchcode_ftp;
				 $(".swindow").css("display","none");
				}else{
				$("#state1").removeClass("switchoff");
				$(".swindow").css("display","");
				switchcode_ftp=1;
				document.alg_setting.ftpEn.value =switchcode_ftp;
				}
		});
		$("#switch2").click(function(){
			if(switchcode_sip==1){
				 $("#state2").addClass("switchoff");
				 switchcode_sip=0;
				 document.alg_setting.sipEn.value =switchcode_sip;
				}else{
				$("#state2").removeClass("switchoff");
				switchcode_sip=1;
				document.alg_setting.sipEn.value =switchcode_sip;
				}
		});
		$("#switch3").click(function(){
			if(switchcode_rtsp==1){
				 $("#state3").addClass("switchoff");
				 switchcode_rtsp=0;
				 document.alg_setting.rtspEn.value =switchcode_rtsp;
				}else{
				$("#state3").removeClass("switchoff");
				switchcode_rtsp=1;
				document.alg_setting.rtspEn.value =switchcode_rtsp;
				}
		});
		$("#switch4").click(function(){
			if(switchcode_l2tp==1){
				 $("#state4").addClass("switchoff");
				 switchcode_l2tp=0;
				 document.alg_setting.l2tpEn.value =switchcode_l2tp;
				}else{
				$("#state4").removeClass("switchoff");
				switchcode_l2tp=1;
				document.alg_setting.l2tpEn.value =switchcode_l2tp;
				}
		});
		
		$("#subbtn").click(function(){
			//if(check_value()==false)
			//	return false;
		Alert("正在保存设置，请稍候", 10);	
		setTimeout("Alert('设置成功，无线网络将断开，请在30秒后重新进入本页面', 20)", 10100);	
		document.alg_setting.submit();
    });
	});

//防止缓存
//window.setInterval("refreshCookie()", 1000);  

</script>
</body>
</html>
