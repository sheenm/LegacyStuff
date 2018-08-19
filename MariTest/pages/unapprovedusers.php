<?	//Страничка с непроверенными пользователями юниона. А также возможностью их верифицироать.
class UnapprovedUsersPage extends ContentPage {

	var $neededRights = array('approve user');

	function DisplayContent() {
		if(!isset($_GET['union'])) {
			$_SESSION['error'] = "Не выбрано объединение";
			header('Location:?page=unapprovedusers&union=' . $_GET['union'] . '&year=' . $_GET['year']);
		}

		$union = $_GET['union'];
		$unapprovedUsers = $this->dbControl->GetUnionUsers($union, $_GET['year'], 0);

		
		if(sizeof($unapprovedUsers) && isset($_POST['formfilled'])) {
			$users = array();

			foreach($unapprovedUsers as $user) {
				if(isset($_POST[$user['login']]) && $_POST[$user['login']] =="on") $users[] = $user['login'];
			}
			$message = $this->dbControl->ApproveUsers($union, $_GET['year'], $users);
			if($message) $_SESSION['error'] = $message;
			header('Location:?page=unapprovedusers&union=' . $_GET['union'] . '&year=' . $_GET['year']);
		}

		echo "<h3>Верификация пользователей</h3>";
		if(!count($unapprovedUsers)) {
			echo 'Нет неверифицированных пользователей';
		} else {
			?>
			
			<form action =<?="\"?page=unapprovedusers&union=".$_GET['union']. '&year=' . $_GET['year'].
			"\""?> method="post" autocomplete="off" enctype="mutipart/form-data">
			<?
			foreach ($unapprovedUsers as $user) {
				echo "<input type=\"checkbox\" checked=\"checked\" name=\"", $user['login'] ,"\"> ",
				$user['name'], ' ', $user['middle_name'], ' ', $user['last_name'], '<br/>';
			}
			?>
			<input type="hidden" name="formfilled" />
			<input type="submit" value="Верифицировать"/>
			</form>
			<?
		}
	}
}	new UnapprovedUsersPage();
?>