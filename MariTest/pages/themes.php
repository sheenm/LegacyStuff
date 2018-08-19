<? //Выводит список тем и действий над ними для конкретного объединения.

class ThemesPage extends ContentPage {

	var $neededRights = array('create theme', 'create question');

	function DisplayContent() {
		$union = $_GET['union'];
		$yearStarted = $_GET['year'];
		$themes = $this->dbControl->GetUnionThemes($union, $yearStarted);
		//if(!count($themes)) return;
		
		?>
		<h3>Темы объединения <?=$union?> 
			(<a href='?page=createtheme&amp;union=<?=$union?>&amp;year=<?=$yearStarted?>'>Добавить тему</a>)</h3>
		<table class="testTable">
			<tr> 
				<th>Название</th>
				<th>Описание</th>
				<th>Вопросы</th>
				<th>Удалить</th>
			</tr>
		<?

		foreach ($themes as $theme) {
			$name = $theme['name'];
			echo 	'<tr>',
						'<td>', $name, '</td>',
						'<td>', $theme['info'] , '</td>',
						'<td>',	
							"<a href='?page=createquestion&union=$union&year=$yearStarted&theme=$name'>Добавить вопрос</a> <br/>", 
							"<a href='?page=questions&union=$union&year=$yearStarted&theme=$name'>Список вопросов</a> <br/>", 
						'</td>',
						'<td>', "<a href='?page=deleteTheme&union=$union&year=$yearStarted&theme=$name'>Удалить тему</a>", '</td>',
					'</tr>';

		}
		echo "</table>";
	}

} new ThemesPage();

?>