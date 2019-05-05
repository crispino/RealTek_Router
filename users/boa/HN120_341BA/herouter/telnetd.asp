<html>
<head>
<meta http-equiv="Pragma" CONTENT="no-cache">
<meta http-equiv="Cache-Control" CONTENT="no-cache">
<meta http-equiv="Expires" CONTENT="-1">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1"> 
<title>开启Telnet功能</title>
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="js/jquery.vegas.min.js"></script>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<link href="css/jquery.vegas.css" rel="stylesheet" type="text/css" />
<link href="css/and.css" rel="stylesheet" type="text/css" />

<script type="text/javascript">
var admin_passwd = '<%getHeInfo("admin_passwd");%>';

$(function() {
  $.vegas({
    src:'images/bodybg.jpg'
  });
});

function sendAuth() {
	var pwd = $("#mpassword").val();
	if(pwd.length == 0) {
		alert("请输入密码！");
        window.location.reload();
		return;
	}
 
	if (admin_passwd != pwd) {
		alert("密码输入错误，请重新输入！");
        window.location.reload();
		return;
	}
	
	$.ajax({
		type: "post",
		url: "/boafrm/heUITelnetd",
		data: pwd,
		success: function(data){
			$("#result").addClass("success");		
			$("#result").text("Telnet开启成功。");
		},
		error: function(data) {
			$("#result").addClass("err");			
			$("#result").text("Telnet开启失败！");
		}
	});
}

function onload()
{
	document.getElementById("mpassword").focus();	
}

$(document).keydown(function(event){ 
    if(event.keyCode == 13){ 
        sendAuth();
    }
});

</script>
<style type="text/css">
	.loginpart .success{ color:#fff;}
</style>
</head>

<body onload="onload()">
<form>
<div class="full">
	<div class="slg"></div>
	<div class="loginpart">
		<input type="text" style="display:none">
		<h2 class="err">请输入和目路由器管理员密码</h2>
		<div class="inparea"><input name="mpassword" id="mpassword" type="password" />
		</div>
		<h3 id="result"></h3>
		<div class="logbtn"><b onclick="javascript:sendAuth()">提交</b></div>
	</div>
</div>
</form>
<div style="position:absolute; bottom:-6px; width:100%; margin:10 auto; height:35px;  font-size:14px; color:#fff; text-align:center;" >
	和目路由器管理员密码输入成功，将成功开启telnet功能。
</div>

</body>
</html>
