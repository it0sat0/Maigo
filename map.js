function initMap() {
	var px = 35.1239654;
	var py = 136.9417741;
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