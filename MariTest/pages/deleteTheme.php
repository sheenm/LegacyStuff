<? //Страница удаления темы
class DeleteThemePage extends ContentPage {
	
	var $neededRights = array('create theme', 'create question');

	function DisplayContent() {
		echo $this->dbControl->DeleteTheme($_GET['union'], $_GET['year'], $_GET['theme']);
	}
}	new DeleteThemePage();
?>