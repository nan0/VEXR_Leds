#include "WebApp.h"

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
   <head>
      <title>VESC Leds</title>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <style>
        body {
          font-family: Roboto;
          height: 100vh; 
          height: calc(var(--vh, 1vh) * 100); 
          width: 100vw; 
          display: flex; 
          align-items: center; 
          justify-content: center; 
          flex-direction: column; 
          text-align: center; 
          margin: 0;
        }

        body > *:not(:first-child) {
          margin-top: 3rem;
        }

        h2 {
          font-size: 3.0rem; 
        }
        .switch {
          position: relative;
          display: inline-block;
          width: 100px;
          height: 54px;
          -webkit-tap-highlight-color: transparent;
        }
        .switch input { 
          opacity: 0;
          width: 0;
          height: 0;
        }
        .switch input:focus{
          outline: none !important;
        }        
        .slider {
          position: absolute;
          cursor: pointer;
          top: 0;
          left: 0;
          right: 0;
          bottom: 0;
          background-color: #ccc;
          transition: .4s;
        }
        .slider:before {
          position: absolute;
          content: "";
          height: 46px;
          width: 46px;
          left: 4px;
          bottom: 4px;
          background-color: white;
          transition: .4s;
        }
        input:checked + .slider {
          background-color: #2196F3;
        }
        input:focus + .slider {
          box-shadow: 0 0 1px #2196F3;
        }
        input:checked + .slider:before {
          transform: translateX(46px);
        }
        .slider.round {
           border-radius: 34px;
        }
        .slider.round:before {
          border-radius: 50%;
        }
        .link {
          text-decoration: none;
          font-size: smaller;
        }
      </style>
      <script>
        function call(path) {
          var xhr = new XMLHttpRequest();
          xhr.open("GET", path, true);
          xhr.send();
        }
        window.addEventListener('load', (event) => {
          // Fixes viewport offset on mobile pages
          let vh = window.innerHeight * 0.01;
          document.documentElement.style.setProperty('--vh', `${vh}px`);

          document.querySelector("#powerBtn").addEventListener('change', () => this.checked ? call("/on") : call("/off"));
          document.querySelector("#linear-animation").addEventListener('click', () => call("/animate/linear"));
          document.querySelector("#accordion-animation").addEventListener('click', () => call("/animate/accordion"));
          document.querySelector("#fixed-animation").addEventListener('click', () => call("/animate/fixed"));
        });
      </script>
   </head>
   <body>
    <h2>VESC Leds</h2>
    <label class="switch">
      <input id="powerBtn" type="checkbox" checked>
      <span class="slider round"></span>
    </label>
    <form id="animations-form">
      <input type="radio" id="linear-animation" name="animation" value="linear" checked>
      <label for="linear-animation">Linear</label>

      <input type="radio" id="accordion-animation" name="animation" value="accordion">
      <label for="accordion-animation">Accordion</label>

      <input type="radio" id="fixed-animation" name="animation" value="fixed">
      <label for="fixed-animation">Fixed</label>
    </form>
    <a class="link" href="/update">Update firmware</a>
   </body>
</html>
)rawliteral";

const char* WebApp::getHtml() {
  return index_html;
}
