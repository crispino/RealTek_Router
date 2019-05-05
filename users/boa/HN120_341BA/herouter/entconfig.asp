<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>互联网设置</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/herouter.js"></script>
<script type="text/javascript" src="js/jquery.js"></script>
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
.cfgleft li a,.cfgleft li a:visited{color:#666; display:block; height:40px;width:170px; }
.cfgleft li a:hover,.cfgleft li a:active{ text-decoration:none;}
.cfgleft li:hover{ border-left:4px #1e9ec5 solid;  font-weight:bold;}
.cfgleft li.on{ border-left:4px #1e9ec5 solid; color:#1e9ec5; font-weight:bold; font-size:14px; background:#fff url(images/arrow1.png) no-repeat 170px center;}
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
.othersetting{ margin-top:40px; border-top:1px #e2e2e2 solid;}
.othersetting h3{ font-size:14px; color:#8b8b8b; line-height:30px; padding-top:10px;}
.othersetting ul{}
.othersetting li{ float:left; padding:5px; }
.othersetting li a{ display:block; height:37px; width:107px; background:#dff6ff; text-align:center; line-height:37px; color:#289ec2; font-size:14px;}
.othersetting li a:hover{ text-decoration:none; background:#e8f8ff; color:#2097bc; }
-->
</style>
<script type="text/javascript">


function checkIpAddr(field, ismask)
{
	if (field.value == "") {
		field.value = field.defaultValue;
		field.focus();
		return false;
	}

	if (isAllNum(field.value) == 0) {
		field.value = field.defaultValue;
		field.focus();
		return false;
	}

	if (ismask) {
		if ((!checkRange(field.value, 1, 0, 255)) ||
				(!checkRange(field.value, 2, 0, 255)) ||
				(!checkRange(field.value, 3, 0, 255)) ||
				(!checkRange(field.value, 4, 0, 255)))
		{
			field.value = field.defaultValue;
			field.focus();
			return false;
		}
	}
	else {
		if ((!checkRange(field.value, 1, 0, 255)) ||
				(!checkRange(field.value, 2, 0, 255)) ||
				(!checkRange(field.value, 3, 0, 255)) ||
				(!checkRange(field.value, 4, 1, 254)))
		{
			field.value = field.defaultValue;
			field.focus();
			return false;
		}
	}
	return true;
}

function check_value()
{
	if (!checkIpAddr(document.entconfig.entconfiglan, false))
	{
		alert("IP 地址非法，请重新输入！");
		return false;
	}
	return true;
}



$(document).ready(function()
{
		$("#subbtn").click(function(){
			if(check_value()==false)
				return false;
				
      $.ajax({
      	type: "post",
        url: "/boafrm/entconfig",     
        data: $("#entconfig").serialize(),    
        success: function(data){
        	//alert("保存成功！");
        },
        error: function(data) {
        	//alert("保存失败！\n" + data);
        }
      })
      
      var newIp = document.entconfig.entconfiglan.value;
      var newUrl = "http://" + newIp + "/main.html";
      parent.getNewLinkValue(newUrl);
      parent.showEntProcess();
    });	
});

</script>
</head>
<form method="post" name="entconfig" id="entconfig" action="/boafrm/entconfig">
<body>
<div class="int">
	<h1>高级设置</h1>
    <div class="cfg">
        <div class="cfgleft">
            <ul>
                <li class="on"><a href="entconfig.asp">局域网设置</a></li>
                <li><a href="DHCPbanding.asp">DHCP设置</a></li>
				<li><a href="MACClone.asp">MAC地址克隆</a></li>
				<li><a href="wirelesschannel.asp">无线信道设置</a></li>
				<li><a href="wirelessMAC.asp">无线MAC地址访问控制</a></li>
				<li><a href="systemtime.asp">系统时间管理</a></li>
				<li><a href="sysupdate.asp">系统升级</a></li>
				<li><a href="restoredefault.asp">恢复出厂设置</a></li>
            </ul>
        </div>
        <div class="cfgright">
            <table width="100%" border="0" cellspacing="0" cellpadding="0">
              <tr>
                <td width="25%">&nbsp;</td>
                <td width="75%">&nbsp;</td>
              </tr>
              <tr>
                <td><span>IP 地址</span></td>
                <td><input class="inp1" type="text" name="entconfiglan" value="<%getInfo("ip-rom");%>" /></td>
              </tr>
            </table>
            <div class="subpart">
                <ul>
                    <li><b id="subbtn">保存</b></li>
                    <li><b class="cancel" onClick="javascript:parent.TINY.box.hide();">取消</b></li>
                </ul>
            </div>
        </div>
    </div>
</div>
</body>
</form>
</html>
