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
        .toggle {
          position: absolute;
          cursor: pointer;
          top: 0;
          left: 0;
          right: 0;
          bottom: 0;
          background-color: #ccc;
          transition: .4s;
        }
        .toggle:before {
          position: absolute;
          content: "";
          height: 46px;
          width: 46px;
          left: 4px;
          bottom: 4px;
          background-color: white;
          transition: .4s;
        }
        input:checked + .toggle {
          background-color: #2196F3;
        }
        input:focus + .toggle {
          box-shadow: 0 0 1px #2196F3;
        }
        input:checked + .toggle:before {
          transform: translateX(46px);
        }
        .toggle.round {
           border-radius: 34px;
        }
        .toggle.round:before {
          border-radius: 50%;
        }
        .link {
          text-decoration: none;
          font-size: smaller;
        }
        slidecontainer {
          width: 100%;
        }

        .slider {
          -webkit-appearance: none;
          width: 100%;
          height: 10px;
          background: #d3d3d3;
          outline: none;
          opacity: 0.7;
          -webkit-transition: .2s;
          transition: opacity .2s;
        }

        .slider:hover {
          opacity: 1;
        }

        .slider::-webkit-slider-thumb {
          -webkit-appearance: none;
          appearance: none;
          width: 25px;
          height: 25px;
          background: #2196F3;
          cursor: pointer;
          border-radius: 20px;
        }

        .slider::-moz-range-thumb {
          width: 25px;
          height: 25px;
          background: #2196F3;
          cursor: pointer;
          border-radius: 20px;
        }
      </style>
      <script>
        function call(path) {
          const xhr = new XMLHttpRequest();
          xhr.open("GET", path, true);
          xhr.send();
        }
        function get(path, callback) {
          const xhr = new XMLHttpRequest();
          xhr.onreadystatechange = () => {
            if (xhr.readyState == 4 && xhr.status == 200){
              callback(xhr.responseText);
            }
          }
          xhr.open("GET", path, true);
          xhr.send();
        }
        window.addEventListener('load', (event) => {
          // Fixes viewport offset on mobile pages
          let vh = window.innerHeight * 0.01;
          document.documentElement.style.setProperty('--vh', `${vh}px`);

          const powerButton = document.querySelector("#powerBtn");
          powerButton.addEventListener('change', () => powerButton.checked ? call("/on") : call("/off"));
          document.querySelector("#linear-animation").addEventListener('click', () => call("/animate/linear"));
          document.querySelector("#accordion-animation").addEventListener('click', () => call("/animate/accordion"));
          document.querySelector("#fixed-animation").addEventListener('click', () => call("/animate/fixed"));
          document.querySelector("#front-brightness").addEventListener('change', e => {
            const level = e.target.value;
            call(`/brightness/front?level=${level}`);
            document.querySelector("#front-brightness-value").innerText=level;
          });
          document.querySelector("#back-brightness").addEventListener('change', e => {
            const level = e.target.value;
            call(`/brightness/back?level=${level}`);
            document.querySelector("#back-brightness-value").innerText=level;
          });
          get("/status/power", switchedOn => {
            document.querySelector("#powerBtn").checked = +switchedOn;
          });
          get("/status/brightness/front", level => {
            document.querySelector("#front-brightness").value = +level;
            document.querySelector("#front-brightness-value").innerText=level;
          });
          get("/status/brightness/back", level => {
            document.querySelector("#back-brightness").value = +level;
            document.querySelector("#back-brightness-value").innerText=level;
          });
          get("/status/animation", animation => {
            document.animationForm.animation.value = animation;
          });
        });
      </script>
   </head>
   <body>
    <h2>VESC Leds</h2>
    <label class="switch">
      <input id="powerBtn" type="checkbox" checked>
      <span class="toggle round"></span>
    </label>
    <div class="slidecontainer">
      <p>
        <label for="front-brightness">Front Brightness (<span id="front-brightness-value">0</span>)</label>
        <input class="slider" id="front-brightness" type="range" min="1" max="255" value="0">
      </p>
      <p>
        <label for="back-brightness">Back Brightness (<span id="back-brightness-value">0</span>)</label>
        <input class="slider" id="back-brightness" type="range" min="1" max="255" value="0">
      </p>
    </div>
    <form id="animations-form" name="animationForm">
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
