<? //Страница регистрации пользователя.

class RegisterPage extends ContentPage {

	function DisplayContent() {

		if(sizeof($_POST)) {
			$userData = array(
				'login'				=> $_POST['login'],
				'password'			=> $_POST['password'],
				'passwordRepeat'	=> $_POST['repeatPassword'],
				'name'				=> $_POST['name'],
				'middleName'		=> $_POST['fatherName'],
				'lastName'			=> $_POST['surname'],
				'code'				=> $_POST['code'],
				'unions'			=> array()
			);
			if($_POST['union'] != '' && $_POST['year'] !='') {
				$_POST['year'] = MyLib::AgesToYear($_POST['year'], 9);
				$userData['unions'] = array($_POST['union'] => $_POST['year']);
			}
					
			$result =  $this->dbControl->RegisterUser($userData);
			if($result == "Пользователь зарегистрирован успешно") {
				$_SESSION['message'] = $result;
				header('Location:?page=login');
			}
			else {  
				$_SESSION['error'] = $result;
				header('Location:?page=register');
			}
		}
		?>	
		<form action ="?page=register" method="post" name="registerForm" autocomplete="off" enctype="mutipart/form-data">
			<span id='errorBox' class='error'></span>
			<table>
				<tr><td><p>Логин</p></td><td><input name="login" type="text" /></td></tr>
				<tr><td><p>Пароль</p></td><td><input name="password" type="password"  /></td></tr>
				<tr><td><p>Повторите пароль</p></td><td><input name="repeatPassword" type="password"  /></td></tr>
				<tr><td><p>Фамилия</p></td><td><input name="surname" type="text" /></td></tr>
				<tr><td><p>Имя</p></td><td><input name="name" type="text" /></td></tr>
				<tr><td><p>Отчество</p></td><td><input name="fatherName" type="text" /></td></tr>
				<tr><td><p>Объединение</p></td><td><select name="union" type="text" > 
														<?
														foreach($this->dbControl->GetUnionsList() as $union) {
															echo "<option>$union</option>";
														}
														?>
													</select></td><td></td></tr>
				<tr><td><p>Год обучения</p></td><td><input name="year" type="number" /></td></tr>
				<tr><td><p>Код учителя</p></td><td><input name="code" type="password" /></td></tr>
				<tr><td></td><td><input name="go" type="submit" value="Зарегистрировать" /></td></tr>
			</table>
		</form>

		<script type="text/javascript">
			document.registerForm.onsubmit = function () {
				if (document.registerForm.login.value == '' ||
					document.registerForm.password.value == '' ||
					document.registerForm.repeatPassword.value == '' ||
					document.registerForm.surname.value == '' ||
					document.registerForm.name.value == '' ||
					document.registerForm.fatherName.value == '' ||
					document.registerForm.union.value == '' || 
					document.registerForm.year.value == '') 
				{
					document.getElementById('errorBox').innerHTML = 'Заполните все поля';
					return false;
				}
				else {
					return true;
				}
			}
		</script>
		<?
	}
} new RegisterPage();
?>