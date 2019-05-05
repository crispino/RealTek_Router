<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" /> 
<title>防火墙管理ALG</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="js/herouter.js"></script>
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
        <div class="cfgright" style="padding-top:10px;padding-left: 12px;">
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
