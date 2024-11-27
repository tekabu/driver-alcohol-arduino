const char home_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Alcohol Detection</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>body { font-family: Arial; text-align: center; }</style>
</head>
<body>
  <h2>Sensor Value</h2>
  <h1 id="sensor"></h1>
  <h1 id="latlng"></h1>
  <script>
      function requestMillis() {
        fetch('/sensor')
          .then(response => response.text())
          .then(data => {
            document.getElementById("sensor").innerText = data;
          })
          .catch(error => console.error("Error fetching sensor:", error));
      }
      function requestLatlng() {
        fetch('/latlng')
          .then(response => response.text())
          .then(data => {
            document.getElementById("latlng").innerText = data;
          })
          .catch(error => console.error("Error fetching latlng:", error));
      }

      setInterval(requestMillis, 2000);
      setInterval(requestLatlng, 5000);
  </script>
</body>
</html>
)rawliteral";

const char contacts_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Alcohol Detection</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>body { font-family: Arial; text-align: center; }</style>
</head>
<body>
  <h2>Contact Numbers</h2>
  <form action="/submit" method="get">
    <label for="mobile1">Mobile Number 1:</label>
    <input type="text" id="mobile1" name="mobile1" value="%MOBILE1%" maxlength="11" placeholder="09123456789"><br><br>

    <label for="mobile2">Mobile Number 2:</label>
    <input type="text" id="mobile2" name="mobile2" value="%MOBILE2%" maxlength="11" placeholder="09123456789"><br><br>

    <label for="mobile3">Mobile Number 3:</label>
    <input type="text" id="mobile3" name="mobile3" value="%MOBILE3%" maxlength="11" placeholder="09123456789"><br><br>

    <label for="mobile4">Mobile Number 4:</label>
    <input type="text" id="mobile4" name="mobile4" value="%MOBILE4%" maxlength="11" placeholder="09123456789"><br><br>

    <label for="mobile5">Mobile Number 5:</label>
    <input type="text" id="mobile5" name="mobile5" value="%MOBILE5%" maxlength="11" placeholder="09123456789"><br><br>

    <button type="submit">Submit</button>
  </form>
  <script>
        // Function to validate mobile number format
        function isValidMobileNumber(number) {
            const mobilePattern = /^09\d{9}$/;
            return mobilePattern.test(number);
        }

        // Add event listener to the form
        document.getElementById("mobileForm").addEventListener("submit", function(event) {
            // Get all input fields
            const inputs = document.querySelectorAll("input[type='text']");
            let isValid = true;

            // Check each input field
            inputs.forEach(input => {
                const value = input.value.trim();
                // Check if there's data and if it doesn't match the format
                if (value.length > 0 && !isValidMobileNumber(value)) {
                    isValid = false;
                }
            });

            // If any input is invalid, show an alert and prevent form submission
            if (!isValid) {
                alert("Please enter a valid mobile number in the format 092XXXXXXX for all fields with data.");
                event.preventDefault();
            }
        });
    </script>
</body>
</html>
)rawliteral";
