<? //index/наше всё
require_once("core/page.php");
require_once("core/db.php");
require_once("core/mylib.php");
require_once("pages/contentpagetemplate.php");

session_start();
if(!isset($_SESSION['userName']) && 
	(!isset($_GET['page']) || !MyLib::IsNameInRange($_GET['page'], array('login','logout', 'register')))) {
	
	$_SESSION['error'] = "Вы не авторизованы.";
	$_GET['page'] = 'login';
}

if(!isset($_GET['page']))
{
	//"Перенаправление на главную:<br/>";
	$_GET['page']='main';
}

new Page();
?>
