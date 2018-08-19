<? //Страница со списком доступныъ объединений и управлением ими
class UnionsPage extends ContentPage {
	var $neededRights = array('create union', 'create question', 'create test');

	function DisplayContent() {

		foreach($_SESSION['unions'] as $union) {
			$yearStarted = MyLib::YearToAges($union['year_started']);
			$union = $union['name'];
			echo "<h3>Объединение $union ($yearStarted год обучения):</h3>
				<a href='?page=unapprovedusers&union=$union&year=$yearStarted'>Неверифицированные пользователи</a> <br/>
				<a href='?page=exitfromunion&union=$union&year=$yearStarted'>Управление пользователями</a> <br/>
				<a href='?page=createtest&union=$union&year=$yearStarted'>Создать тест</a><br/>
				<a href='?page=themes&union=$union&year=$yearStarted'>Темы</a>
				<h4>Список группы:</h4>
			";

			$users = $this->dbControl->GetUnionUsers($union, $yearStarted, 1);
			foreach($users as $user) {
				echo 	'<a href="?page=useruniontests&user=', $user['login'], '&union=', $union, '&year=', $yearStarted, '" >',
						$user['name'], ' ', $user['middle_name'], ' ', $user['last_name'], 
						'</a><br/>';
			}
		}
	}
} new UnionsPage();

?>