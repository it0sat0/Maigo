function initMap() {
	var px = 34.704166;
	var py = 135.494943;
	var opts = {
		zoom: 15,
		center: new google.maps.LatLng(px, py)
	};
	var map = new google.maps.Map(document.getElementById("map"), opts);
	var marker = new google.maps.Marker({
		position: new google.maps.LatLng(px, py),
        map: map
     });
}