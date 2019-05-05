<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>互联网设置</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/herouter.js"></script>
<style type="text/css">
<!--
body{ color:#676767;}
*{-moz-transition: all 0.3s ease-in;-webkit-transition: all 0.3s ease-in;-o-transition: all 0.3s ease-in;transition: all 0.3s ease-in;}
*:hover{-moz-transition: all 0.3s ease-in;-webkit-transition: all 0.3s ease-in;-o-transition: all 0.3s ease-in;transition: all 0.3s ease-in;}
.int { height:410px; width:670px; overflow:hidden; background:#fff url(images/fullbg.gif) repeat-x 0 -2px; padding:5px 15px;}
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
.cfgleft li.on a{color:#1e9ec5;}
.cfgright{ float:right; width:470px; height:330px;}
.cfgright td{ height:36px; font-size:14px;}
.cfgright td span{ padding-left:50px; font-size:14px;}
.cfgright td span2{ padding-left:50px; font-size:14px; display:-moz-inline-box; display:inline-block; width:100px;}
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



.selectinp{height:28px; border:1px #d2d2d2 solid; width:60px; line-height:26px; font-family:"微软雅黑";color:#666;padding:0 10px; background:#fff url(images/dropd.png) no-repeat right center; margin-right:5px;}
.fix{ position:absolute;}
.bm {margin-top:30px;position:absolute;}
.ganyin{ position:absolute; height:30px;cursor:pointer; border:1px #333 solid; z-index:100001; background:#0099CC;filter:alpha(opacity=0);-moz-opacity:0;-khtml-opacity: 0;opacity: 0;}
.bm ul{ padding:1px 0 0 0;  border-left:1px #eee solid; border-right:1px #eee solid; }
.bm li{ color:#0781d9; line-height:30px; padding-left:10px; height:30px; background:#fff; font-size:12px; text-align:left; padding-right:12px; display:block;}
.bm .nowon{ color:#adadae;}
.bm li:hover{ background-color:#b2e9fd;cursor:pointer;}

-->
</style>
</head>

<body style="overflow:hidden" onLoad="init();">
<div class="int">
	<h1>高级设置</h1>
    <div class="cfg">
        <div class="cfgleft">
            <ul>
                <li><a href="WANConfig.asp">WAN设置</a></li>
				<li><a href="DHCPbanding.asp">局域网设置</a></li>
				<li><a href="MACClone.asp">MAC地址克隆</a></li>
				<li class="on"><a href="systemtime.asp">系统时间管理</a></li>
				<li><a href="sysupdate.asp">系统升级</a></li>
				<li><a href="restoredefault.asp">恢复出厂设置</a></li>
            </ul>
        </div>
        <form method="post" name="settime" action="/boafrm/setSysTime">
        <div class="cfgright">
	        	<table width="100%" border="0" cellspacing="0" cellpadding="0">
	        	  <tr>
	                <td width="35%" >
	                <input class="radiobox" type="radio" name="time_radio" id="time_radio" onClick="switch_setType()"  value="0">手动设置
	                </td>
	                <td width="35%" >                      				
					<input class="radiobox" type="radio" name="time_radio" id="time_radio" onClick="switch_setType()"  value="1">自动同步							
	                </td>
	                <td>&nbsp;</td>
	            </tr>
	          </table>
            <table width="100%" border="0" cellspacing="0" cellpadding="0">
              <tr id="curtime">
                <td colspan="8">当前系统时间：<% getHeInfo("he_system_time_get"); %> </td>
              </tr>
              <tr id="manNTPServer">
							  <td><span2>NTP服务器</span2></td>
							  <td colspan="6"><input class="inp1" size="32" maxlength="64" id="NTPServerIP" name="NTPServerIP" <% getHeInfo("he_ntp_server_get"); %> type="text">
							  </td>
							  <td>
							  	<br>&nbsp;&nbsp;<font color="#808080">例：&nbsp;time.nist.gov</font>
									<br>&nbsp;&nbsp;<font color="#808080">&nbsp;&nbsp;&nbsp;&nbsp;ntp0.broad.mit.edu</font>
									<br>&nbsp;&nbsp;<font color="#808080">&nbsp;&nbsp;&nbsp;&nbsp;time.stdtime.gov.tw</font>
							  </td>
							</tr>
							<tr id="manNTPSync">
							  <td><span2>NTP同步(小时)</span2></td>
							  <td colspan="6"><input class="inp1" size="4" id="NTPSync" name="NTPSync" <% getHeInfo("he_ntp_sync_get"); %> type="text"> </td>
							</tr>
              <tr id="sysdate">
                <td><span>修改为</span></td>
                <td colspan="6"><input id="ymd" name="ymd" class="inp1" type="text"></td>
              </tr>
              <tr id="systime">
                <td width="28%"></td>
                <td width="14%">
				<div class="fix" style=" z-index:999;">
					<div class="ganyin" onclick="hide('test1')" style=" width:80px;"></div>
						<div id="test1" style="display: none; height:150px; overflow-y:auto; overflow-x:hidden; border-bottom:1px #eee solid;" class="bm">
						<ul style=" width:80px;">
							<li id="h0" onclick="pick('0','select1','test1')" >0</li>
							<li id="h1" onclick="pick('1','select1','test1')" >1</li>
							<li id="h2" onclick="pick('2','select1','test1')" >2</li>
							<li id="h3" onclick="pick('3','select1','test1')" >3</li>
							<li id="h4" onclick="pick('4','select1','test1')" >4</li>
							<li id="h5" onclick="pick('5','select1','test1')" >5</li>
							<li id="h6" onclick="pick('6','select1','test1')" >6</li>
							<li id="h7" onclick="pick('7','select1','test1')" >7</li>
							<li id="h8" onclick="pick('8','select1','test1')" >8</li>
							<li id="h9" onclick="pick('9','select1','test1')" >9</li>
							<li id="h10" onclick="pick('10','select1','test1')" >10</li>
							<li id="h11" onclick="pick('11','select1','test1')" >11</li>
							<li id="h12" onclick="pick('12','select1','test1')" >12</li>
							<li id="h13" onclick="pick('13','select1','test1')" >13</li>
							<li id="h14" onclick="pick('14','select1','test1')" >14</li>
							<li id="h15" onclick="pick('15','select1','test1')" >15</li>
							<li id="h16" onclick="pick('16','select1','test1')" >16</li>
							<li id="h17" onclick="pick('17','select1','test1')" >17</li>
							<li id="h18" onclick="pick('18','select1','test1')" >18</li>
							<li id="h19" onclick="pick('19','select1','test1')" >19</li>	
							<li id="h20" onclick="pick('20','select1','test1')" >20</li>
							<li id="h21" onclick="pick('21','select1','test1')" >21</li>
							<li id="h22" onclick="pick('22','select1','test1')" >22</li>
							<li id="h23" onclick="pick('23','select1','test1')" >23</li>																																		
						</ul>
					</div>
				</div>
				<input class="selectinp" type="text" id="select1" name="hour" value="0"/></td>
                <td width="6%">时</td>
                <td width="14%">
				<div class="fix" style=" z-index:999;">
					<div class="ganyin" onclick="hide('test2')" style=" width:80px;"></div>
						<div id="test2" style="display: none; height:150px; overflow-y:auto; overflow-x:hidden; border-bottom:1px #eee solid;" class="bm">
						<ul style=" width:80px;">
							<li id="m0" onclick="pick('0','select2','test2')" >0</li>
							<li id="m1" onclick="pick('1','select2','test2')" >1</li>	
							<li id="m2" onclick="pick('2','select2','test2')" >2</li>
							<li id="m3" onclick="pick('3','select2','test2')" >3</li>
							<li id="m4" onclick="pick('4','select2','test2')" >4</li>			
							<li id="m5" onclick="pick('5','select2','test2')" >5</li>
							<li id="m6" onclick="pick('6','select2','test2')" >6</li>	
							<li id="m7" onclick="pick('7','select2','test2')" >7</li>
							<li id="m8" onclick="pick('8','select2','test2')" >8</li>
							<li id="m9" onclick="pick('9','select2','test2')" >9</li>	
							<li id="m10" onclick="pick('10','select2','test2')" >10</li>
							<li id="m11" onclick="pick('11','select2','test2')" >11</li>	
							<li id="m12" onclick="pick('12','select2','test2')" >12</li>
							<li id="m13" onclick="pick('13','select2','test2')" >13</li>
							<li id="m14" onclick="pick('14','select2','test2')" >14</li>			
							<li id="m15" onclick="pick('15','select2','test2')" >15</li>
							<li id="m16" onclick="pick('16','select2','test2')" >16</li>	
							<li id="m17" onclick="pick('17','select2','test2')" >17</li>
							<li id="m18" onclick="pick('18','select2','test2')" >18</li>
							<li id="m19" onclick="pick('19','select2','test2')" >19</li>	
							<li id="m20" onclick="pick('20','select2','test2')" >20</li>	
							<li id="m21" onclick="pick('21','select2','test2')" >21</li>
							<li id="m22" onclick="pick('22','select2','test2')" >22</li>
							<li id="m23" onclick="pick('23','select2','test2')" >23</li>			
							<li id="m24" onclick="pick('24','select2','test2')" >24</li>
							<li id="m25" onclick="pick('25','select2','test2')" >25</li>	
							<li id="m26" onclick="pick('26','select2','test2')" >26</li>
							<li id="m27" onclick="pick('27','select2','test2')" >27</li>
							<li id="m28" onclick="pick('28','select2','test2')" >28</li>	
							<li id="m29" onclick="pick('29','select2','test2')" >29</li>	
							<li id="m30" onclick="pick('30','select2','test2')" >30</li>
							<li id="m31" onclick="pick('31','select2','test2')" >31</li>
							<li id="m32" onclick="pick('32','select2','test2')" >32</li>			
							<li id="m33" onclick="pick('33','select2','test2')" >33</li>
							<li id="m34" onclick="pick('34','select2','test2')" >34</li>	
							<li id="m35" onclick="pick('35','select2','test2')" >35</li>
							<li id="m36" onclick="pick('36','select2','test2')" >36</li>
							<li id="m37" onclick="pick('37','select2','test2')" >37</li>	
							<li id="m38" onclick="pick('38','select2','test2')" >38</li>	
							<li id="m39" onclick="pick('39','select2','test2')" >39</li>
							<li id="m40" onclick="pick('40','select2','test2')" >40</li>
							<li id="m41" onclick="pick('41','select2','test2')" >41</li>			
							<li id="m42" onclick="pick('42','select2','test2')" >42</li>
							<li id="m43" onclick="pick('43','select2','test2')" >43</li>	
							<li id="m44" onclick="pick('44','select2','test2')" >44</li>
							<li id="m45" onclick="pick('45','select2','test2')" >45</li>
							<li id="m46" onclick="pick('46','select2','test2')" >46</li>	
							<li id="m47" onclick="pick('47','select2','test2')" >47</li>	
							<li id="m48" onclick="pick('48','select2','test2')" >48</li>
							<li id="m49" onclick="pick('49','select2','test2')" >49</li>
							<li id="m50" onclick="pick('50','select2','test2')" >50</li>			
							<li id="m51" onclick="pick('51','select2','test2')" >51</li>
							<li id="m52" onclick="pick('52','select2','test2')" >52</li>	
							<li id="m53" onclick="pick('53','select2','test2')" >53</li>
							<li id="m54" onclick="pick('54','select2','test2')" >54</li>
							<li id="m55" onclick="pick('55','select2','test2')" >55</li>	
							<li id="m56" onclick="pick('56','select2','test2')" >56</li>	
							<li id="m57" onclick="pick('57','select2','test2')" >57</li>
							<li id="m58" onclick="pick('58','select2','test2')" >58</li>
							<li id="m59" onclick="pick('59','select2','test2')" >59</li>																																																																																						
						</ul>
					</div>
				</div>
				<input class="selectinp" type="text" id="select2" name="minute" value="0"/></td>
                <td width="6%">分</td>
                <td width="14%">
				<div class="fix" style=" z-index:999;">
					<div class="ganyin" onclick="hide('test3')" style=" width:80px;"></div>
						<div id="test3" style="display: none; height:150px; overflow-y:auto; overflow-x:hidden; border-bottom:1px #eee solid;" class="bm">
						<ul style=" width:80px;">
							<li id="s0" onclick="pick('0','select3','test3')" >0</li>
							<li id="s1" onclick="pick('1','select3','test3')" >1</li>	
							<li id="s2" onclick="pick('2','select3','test3')" >2</li>
							<li id="s3" onclick="pick('3','select3','test3')" >3</li>
							<li id="s4" onclick="pick('4','select3','test3')" >4</li>			
							<li id="s5" onclick="pick('5','select3','test3')" >5</li>
							<li id="s6" onclick="pick('6','select3','test3')" >6</li>	
							<li id="s7" onclick="pick('7','select3','test3')" >7</li>
							<li id="s8" onclick="pick('8','select3','test3')" >8</li>
							<li id="s9" onclick="pick('9','select3','test3')" >9</li>	
							<li id="s10" onclick="pick('10','select3','test3')" >10</li>
							<li id="s11" onclick="pick('11','select3','test3')" >11</li>	
							<li id="s12" onclick="pick('12','select3','test3')" >12</li>
							<li id="s13" onclick="pick('13','select3','test3')" >13</li>
							<li id="s14" onclick="pick('14','select3','test3')" >14</li>			
							<li id="s15" onclick="pick('15','select3','test3')" >15</li>
							<li id="s16" onclick="pick('16','select3','test3')" >16</li>	
							<li id="s17" onclick="pick('17','select3','test3')" >17</li>
							<li id="s18" onclick="pick('18','select3','test3')" >18</li>
							<li id="s19" onclick="pick('19','select3','test3')" >19</li>	
							<li id="s20" onclick="pick('20','select3','test3')" >20</li>	
							<li id="s21" onclick="pick('21','select3','test3')" >21</li>
							<li id="s22" onclick="pick('22','select3','test3')" >22</li>
							<li id="s23" onclick="pick('23','select3','test3')" >23</li>			
							<li id="s24" onclick="pick('24','select3','test3')" >24</li>
							<li id="s25" onclick="pick('25','select3','test3')" >25</li>	
							<li id="s26" onclick="pick('26','select3','test3')" >26</li>
							<li id="s27" onclick="pick('27','select3','test3')" >27</li>
							<li id="s28" onclick="pick('28','select3','test3')" >28</li>	
							<li id="s29" onclick="pick('29','select3','test3')" >29</li>	
							<li id="s30" onclick="pick('30','select3','test3')" >30</li>
							<li id="s31" onclick="pick('31','select3','test3')" >31</li>
							<li id="s32" onclick="pick('32','select3','test3')" >32</li>			
							<li id="s33" onclick="pick('33','select3','test3')" >33</li>
							<li id="s34" onclick="pick('34','select3','test3')" >34</li>	
							<li id="s35" onclick="pick('35','select3','test3')" >35</li>
							<li id="s36" onclick="pick('36','select3','test3')" >36</li>
							<li id="s37" onclick="pick('37','select3','test3')" >37</li>	
							<li id="s38" onclick="pick('38','select3','test3')" >38</li>	
							<li id="s39" onclick="pick('39','select3','test3')" >39</li>
							<li id="s40" onclick="pick('40','select3','test3')" >40</li>
							<li id="s41" onclick="pick('41','select3','test3')" >41</li>			
							<li id="s42" onclick="pick('42','select3','test3')" >42</li>
							<li id="s43" onclick="pick('43','select3','test3')" >43</li>	
							<li id="s44" onclick="pick('44','select3','test3')" >44</li>
							<li id="s45" onclick="pick('45','select3','test3')" >45</li>
							<li id="s46" onclick="pick('46','select3','test3')" >46</li>	
							<li id="s47" onclick="pick('47','select3','test3')" >47</li>	
							<li id="s48" onclick="pick('48','select3','test3')" >48</li>
							<li id="s49" onclick="pick('49','select3','test3')" >49</li>
							<li id="s50" onclick="pick('50','select3','test3')" >50</li>			
							<li id="s51" onclick="pick('51','select3','test3')" >51</li>
							<li id="s52" onclick="pick('52','select3','test3')" >52</li>	
							<li id="s53" onclick="pick('53','select3','test3')" >53</li>
							<li id="s54" onclick="pick('54','select3','test3')" >54</li>
							<li id="s55" onclick="pick('55','select3','test3')" >55</li>	
							<li id="s56" onclick="pick('56','select3','test3')" >56</li>	
							<li id="s57" onclick="pick('57','select3','test3')" >57</li>
							<li id="s58" onclick="pick('58','select3','test3')" >58</li>
							<li id="s59" onclick="pick('59','select3','test3')" >59</li>	
						</ul>
					</div>
				</div>
				<input class="selectinp" type="text" id="select3" name="second" value="0"/></td>
                <td width="19%" >秒</td>
              </tr>
            </table>
            <div class="subpart">
                <ul>
                    <li><b onclick="setTimeSubmit();">保存</b></li>
                    <li><b class="cancel" onClick="javascript:parent.TINY.box.hide();">取消</b></li>
                </ul>
            </div>
			
		<input type="hidden" value="/herouter/systemtime.asp" name="submit-url">
        </div>
        </form>
    </div>
</div>

<script type="text/javascript">
	
var mode = "<% getHeInfo("NTPEnable"); %>";

function init(){
  
  if (mode == "0")
  {      
  	document.settime.time_radio[0].checked = true; 
  	AutoChoose(0);
	}     
	else
	{
		document.settime.time_radio[1].checked = true;
		AutoChoose(1);
	}

	return true;
}
	
function AutoChoose(isAuto){

    var visitTime;
    var visitNtp;
    
    if(isAuto == 0){
    	  visitTime = "block";
    	  visitNtp = "none";

    }else if(isAuto == 1){
    	  visitTime = "none";
    	  visitNtp = "block";
    }
        
    document.getElementById("sysdate").style.display = visitTime;
    document.getElementById("systime").style.display = visitTime;
    document.getElementById("manNTPServer").style.display = visitNtp;
    document.getElementById("manNTPSync").style.display = visitNtp;
}
 
function switch_setType(){
	
	if (document.settime.time_radio[0].checked == true){
      AutoChoose(0);
  }
	else if(document.settime.time_radio[1].checked == true){
      AutoChoose(1);
  }
}	

function setTimeSubmit()
{
	if (checkFormData())
	{
		document.settime.submit();
		Alert("正在保存设置，请稍候......", 5);
	}
}

function checkFormData()
{   
    var dateStr;
    var serverStr;
    var syncStr;
		
	if (document.settime.time_radio[0].checked == true)
	{
		dateStr = document.getElementById("ymd").value;
		if (dateStr == "")
		{
			alert("日期不能为空!");
			return false;
		}
		
		if (!checkDate(dateStr))
		{
			alert("输入的日期格式不对! 日期格式为：YYYY-MM-DD，例：2014-01-01");
			//document.getElementById("ymd").focus();
			return false;
		}
		
		if (!checkMonthDay(dateStr))
		{
			alert("无效的日期!");
			return false;
		}
	}
	else if (document.settime.time_radio[1].checked == true)
	{
			if (!NTPFormCheck())
				return false;
	}
	
	return true;
}

function checkDate(ds) {
    var reg = /^(\d{1,4})-(\d{1,2})-(\d{1,2})$/
    var mr = reg.exec(ds);
    if (!mr) return false;

		return true;
}

function checkMonthDay(ds) {
	var str = new Array();
	str = ds.split("-");
	var year = str[0];
    var month = str[1];
    var day = str[2];
    var myDate = new Date();
	
	if (year < 1900 || year > myDate.getFullYear())
		return false;
		
    if (month < 1 || month > 12)
    	return false;
    	
    if (day < 1 || day > 31)
    	return false;

		return true;
}

function NTPFormCheck()
{
	if(document.getElementById("NTPServerIP").value != "" && 
		document.getElementById("NTPSync").value == ""){
		alert("请指定用于同步的时间!");
		return false;
	}
	if(isAllNum(document.getElementById("NTPSync").value) == 0){
		alert("无效的NTP同步时间!");
		return false;
	}
	if(atoi(document.getElementById("NTPSync").value, 1) > 300 || atoi(document.getElementById("NTPSync").value, 1) == 0){
		alert("同步时间值错误!(1~300)");
		return false;
	}		
	return true;
}

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

</script>
</body>
</html>
