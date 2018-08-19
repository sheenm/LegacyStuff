<? // Класс страницы. Главная движуха вокруг него:)
class Page {
	var $title = "Cистема тестирования знаний в дополнительном образовании";
	var $footerMsg  = "СПбЦД(Ю)ТТ";
	var $loadType;
	
	function Page($fullReload='full') {
		$this->loadType = $fullReload;
		$this->Show();
	}
	
	function Show() {
		switch ($this->loadType) {
			case 'full':?>
				<!DOCTYPE html>
				<!--fullReload-->
				<html>
				<head>
					<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
					<title><?=$this->title?></title>
					<link rel="stylesheet" type="text/css" href="css/global.css"/>
					<link rel="stylesheet" type="text/css" href="css/table.css"/>
					<script type="text/javascript" src='js/main.js'></script>
					<script type="text/javascript" src='js/prototype.js'></script>
				</head>
				<body>
					<div id="wrap">
						<header>
							<p id="text-header"><?=$this->title?></p>
						</header>
						<div id="page">
							<?if(isset($_SESSION['userName'])) {?>
							<div id="menu">
								<a href='?page=userprofile'>Профиль</a><br/>
								<a href='?page=tests'>Пройти тест</a><br/>
								<?
								if(MyLib::IsNameInRange('create union', $_SESSION['rights'])) {
									echo 
									"<a href='?page=unions'> Объединения</a></br>
									<a href='?page=createunion'>Добавить объединение</a><br/>";
								}
								?>
								<a href='?page=logout'>Выход</a><br/>
							</div>
							<div id="content"><?}?>
								<?=$this->GetContent()?>
							<?if(isset($_SESSION['userName'])){?></div><?}?>
						</div> <!--page-->
					</div> <!--wrap-->
					<footer><?=$this->footerMsg?></footer>
				</body>
				</html><?
				break;
			case 'body':?>
				<!--bodyReload-->
				<div id="wrap">
					<div id="page">
						<?=$this->GetContent()?>
					</div> <!--page-->
				</div> <!--wrap-->
				<footer><?=$this->footerMsg?></footer><?
				break;
			case 'content':
				echo "<!--contentReload-->";
				echo $this->GetContent();
				break;
			default:
				echo "Ошибка загрузки страницы. Попробуйте перезагрузить.";
				break;
		}
	}

	function GetContent() {	
		if(isset($_SESSION['message']))  {
			echo $_SESSION['message'];
			unset($_SESSION['message']);
		}
		if (isset($_SESSION['error'])) {
			echo "<span class='error'>",$_SESSION['error'],"</span>";
			unset($_SESSION['error']);
		}

		$page = "pages/" . $_GET['page'] . ".php";
		if(file_exists($page)) {
			include_once($page);
		}
		else {
			echo "Данная страница не найдена";
		}
		
	}
}