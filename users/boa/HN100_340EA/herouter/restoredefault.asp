<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>互联网设置</title>
<link href="css/style_main.css" rel="stylesheet" type="text/css" />
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.js"></script>
<style type="text/css">
<!--

.recover{ padding:30px 10px 0 0;height:90px; }
.recoverarea{ height:90px; border-bottom:1px #d3d3d3 solid; padding-top:10px;}
.recoverarea p{ float:left; display:block; color:#f00; width:240px; line-height:48px; padding:0px 0 0 35px; background:url(images/check3_07.png) no-repeat 2px center;}
.recoverarea .subbtn1{ float:right;}
.recoverarea .subbtn1 span{display:block; cursor:pointer; height:48px; cursor:pointer; text-align:center; width:180px; line-height:48px; font-size:16px;background:#77d1df; color:#fff;}
.recoverarea .subbtn1 span:hover{background:#56bbcb;}

-->
</style>
</head>
<script language="JavaScript" type="text/javascript">

function restore()
{
	document.getElementById("RestoreDefault").submit();
	parent.showrestoring();
}

$(function(){
	var currOpmode = "<%getHeInfo("opMode")%>";
    if(currOpmode==1){
		$('.ifdisabled').addClass('disabled');
		
		$('.disabled').click(function(event){  
	        return false;  
	    });
	}
	
})

</script>

<body>
<div class="int">
	<h1>高级设置</h1>
    <div class="cfg">
        <div class="cfgleft">
            <ul>
                <li><a class="ifdisabled" href="WANConfig.asp">WAN设置</a></li>
				<li><a href="DHCPbanding.asp">局域网设置</a></li>
				<li><a class="ifdisabled" href="MACClone.asp">MAC地址克隆</a></li>
				<li><a class="ifdisabled" href="systemtime.asp">系统时间管理</a></li>
				<li><a href="sysupdate.asp">系统升级</a></li>
				<li class="on"><a href="restoredefault.asp">恢复出厂设置</a></li>
            </ul>
        </div>
        <div class="cfgright">
			<div class="recover">
				<form method="post" name="RestoreDefault" id="RestoreDefault" action="/boafrm/LoadDefaultSettings">
					<input type="hidden" name="LoadDefault" value="Load+Default"/>
				<div class="recoverarea">
					<p>恢复出厂设置后，您之前的设置将被清空</p>
					<div class="subbtn1">
						<span onClick="restore();">确定</span>
					</div>
				</div>
				</form>
			</div>
        </div>
    </div>
</div>
</body>
</html>
