<?
class LoginPage extends ContentPage {
	function DisplayContent() {
		
		if(isset($_POST['login']) && isset($_POST['password'])) {
		
			if($this->dbControl->CheckUserPassword($_POST['login'], $_POST['password'])) {
				$_SESSION['userName'] = $_POST['login'];
				$_SESSION['rights'] = $this->dbControl->GetUserRights($_POST['login']);
				$_SESSION['unions'] = $this->dbControl->GetUserUnions($_POST['login']);
				header('Location:?page=main');
			}
			else {
				$_SESSION['error'] = "Ошибка процедуры аутентификации";
				header('Location:?page=login');	
			}
		}
		else if (isset($_SESSION['userName'])) header('Location:?page=main');

		?>
		<div style='height:150px'></div>
		<form action ="?page=login" method="post" autocomplete="off" enctype="mutipart/form-data">
			<table>
				<tr>
					<td><p>Логин</p></td>
					<td><input name="login" type="text" /></td>
				</tr>
				<tr>
					<td><p>Пароль</p></td>
					<td><input name="password" type="password"  /></td>
				</tr>
				<tr>
					<td></td>
					<td>
						<input value="Войти" type="submit"/>
					</td>
				</tr>
				<tr>
					<td></td>
					<td><a href ="?page=register">Регистрация</a></td>
				</tr>
			</table>
		</form>
		<?
	}
} new LoginPage();