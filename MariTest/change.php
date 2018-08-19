<?
require_once("core/db.php");

$query = "UPDATE unions
		  SET name = \"Подготовка к олимпиаде Юный космонавт\"
		  WHERE id=8";



$dbControl = new DB();

mysqli_query($dbControl->mysqlConnect,$query);

?>