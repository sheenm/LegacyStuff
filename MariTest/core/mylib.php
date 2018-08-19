<? //Сборник полезных функций

class MyLib {

	/**
	 * проверяет, на совпадение name на элемент из range
	 **/
	static function IsNameInRange($name, $range)
	{
		foreach ($range as $elem) {
			if($name == $elem) return true;
		}
		return false;
	}

	static function EscapeHTMLTags($str) {
		$str = str_replace("<","&lt;", $str);
		$str = str_replace(">","&gt;", $str);
		return str_replace("\"","&quot;", $str);
	}

	static function MySqlQueryToArray($mysqlQuery, $eachResultIsArray=0) {
		$results = array();
		if($mysqlQuery) $numResults = mysqli_num_rows ($mysqlQuery);
		else return $results;

		if(!$eachResultIsArray) {
			for ($i = 0; $i < $numResults; $i++) {
				$elem  =  mysqli_fetch_array($mysqlQuery);
				$results[] = $elem[0];

			}
		} else {
			for ($i = 0; $i < $numResults; $i++) {
				$results[] = mysqli_fetch_array($mysqlQuery);
			}
		}
		return $results;
	}

	//Функции перевода количества лет в год начала и наоборот
	static function AgesToYear($ages, $month = 9) {
		$year = date("Y") - abs($ages);

		if (date("n") >= $month) {
			$year++;
		}
		return $year;
	}

	static function YearToAges($year, $month = 9) {
		$ages = date("Y") - $year;

		if(date("n") >= $month) {
			$ages++;
		}
		return $ages;
	}

}

?>