<?
class UserProfile extends ContentPage {

	var $neededRights = array();

	function DisplayContent() {
		$userInfo = $this->dbControl->GetUserInfo($_SESSION['userName']);
		$userTests = $this->dbControl->GetUserPassedTests($_SESSION['userName']);
		
		echo "<h3>", $userInfo['name'], ' ', $userInfo['middle_name'], ' ', $userInfo['last_name'], '</h3>';
		?>
		<h3>Список ваших объединений</h3>
		<?
			foreach ($this->dbControl->GetUserUnions($_SESSION['userName']) as $union) {
				echo $union['name'], " (", MyLib::YearToAGes($union['year_started'],9), " год обучения) <br/>";
			}

		?>

		<a href='?page=adduserunion'> Присоединиться к объединению</a></br>
		
		<h3>Список пройденных тестов</h3>
		<?
			if(sizeof($userTests)) {
				?>
				<table class="testTable">
					<tr> 
						<th>Название теста</th>
						<th>Объединение</th>
						<th>Тема</th>
						<th>Кол-во вопросов</th>
						<th>Правильных</th>
						<th>Дата прохождения</th>
					</tr>
			
				<?
					foreach(array_reverse($userTests) as $test) {
						echo "<tr>",
								"<td>", $test['name'], "</td>",
								"<td>", $test['unionName'], "</td>",
								"<td>", $test['theme'], "</td>",
								"<td>", $test['questionsNum'], "</td>",
								"<td>", $test['right_answers'], "</td>",
								"<td>", $test['date_passed'], "</td>",
							"</tr>";
					}
				?>
				</table>
				<?
			}
			else {
				echo "Вы не проходили никаких тестов";
			}
		?>
		

			
		<?
	}

}	new UserProfile();
?>