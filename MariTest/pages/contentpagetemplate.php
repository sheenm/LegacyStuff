<? //Класс, который должны наследовать все страницы. 
class ContentPage {
	var $neededRights = array();
	var $dbControl;

	function ContentPage() {
		$this->dbControl = new DB();
		foreach($this->neededRights as $neededRight) {
			if(!MyLib::IsNameInRange($neededRight, $_SESSION['rights'])) {
				echo "Нет права $neededRight для просмотра этой страницы";
				return;
			}
		}

		//Проверка на объединение.
		if(isset($_GET['union']) && isset($_GET['year'])) {
			
			for($i = 0; $i < sizeof($_SESSION['unions']); $i++){
				if($_SESSION['unions'][$i]['name'] == $_GET['union']
				&&	MyLib::YearToAges($_SESSION['unions'][$i]['year_started'],9) == $_GET['year'])
					break;
			}

			if($i == sizeof($_SESSION['unions'])) {
				echo "Вы не входите в это объединение";
				return;
			}
		}
		$this->DisplayContent();
	}

	function DisplayContent() {}

}
?>