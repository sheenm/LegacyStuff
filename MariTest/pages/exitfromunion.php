<? //ПОзволяет выйти из объединения
class ExitFromUnion extends ContentPage {

	var $neededRights = array('approve user', 'create union');

	function DisplayContent() 
	{
		if(!isset($_GET['union'])) {
			$_SESSION['error'] = "Не выбрано объединение";
			header('Location:?page=exitfromunion&union=' . $_GET['union'] . '&year=' . $_GET['year']);
		}

		$union = $_GET['union'];
		$approvedUsers = $this->dbControl->GetUnionUsers($union, $_GET['year'], 1);

		if(sizeof($approvedUsers) && isset($_POST['formfilled'])) {
			$users = array();

			foreach($approvedUsers as $user) {
				if(isset($_POST[$user['login']]) && $_POST[$user['login']] =="on") $users[] = $user['login'];
			}
			$message = $this->dbControl->ApproveUsers($union, $_GET['year'], $users, 0);	
			if($message) $_SESSION['error'] = $message;
			header('Location:?page=exitfromunion&union=' . $_GET['union'] . '&year=' . $_GET['year']);
		}

		echo "<h3>Исключение из объединения</h3>";
		if(!count($approvedUsers)) {
			echo 'Нет пользователей';
		} else {
			?>
			<form action =<?="\"?page=exitfromunion&union=".$_GET['union']. '&year=' . $_GET['year'].
			"\""?> method="post" autocomplete="off" enctype="mutipart/form-data">
			<?
			foreach ($approvedUsers as $user) {
				echo "<input type=\"checkbox\" checked=\"checked\" name=\"", $user['login'] ,"\"> ",
				$user['name'], ' ', $user['middle_name'], ' ', $user['last_name'], '<br/>';
			}
			?>
			<input type="hidden" name="formfilled" />
			<input type="submit" value="Исключить"/>
			</form>
			<?
		}
	}
} new ExitFromUnion();


?>