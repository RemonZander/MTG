import React from "react";
import { useState, useEffect } from 'react';
import './tailwind.css'

function App() 
{
  const [Grid, setGrid] = useState();
  let result = {};

  return (
    <div>
      <button className="w-fit rounded-lg bg-blue-700 p-[5px] m-[10px]" onClick={() => {
        let input = document.createElement('input');
        input.type = 'file';
        input.accept = 'application/json';
        input.onchange = _ => {
          let file = Array.from(input.files)[0]
          let reader = new FileReader();
          reader.readAsText(file);
          reader.onload = function(e) {
            result = JSON.parse(e.target.result)
            console.log("File content: ", result);
          };
          
        };
        input.click()
      }}>
        LoadFile
      </button>

      <button onClick={() => {setGrid(MakeGrid(result))}}>MakeGrid</button>
      {Grid == null ? null : <div className="flex min-w-full justify-center"><div className="flex flex-wrap max-w-[90vw] gap-y-[20px] w-fit">{Grid}</div></div>}
      
    </div>
  )
}

function MakeGrid(result)
{
  let content = []
  for (let k = 0; k < result.steps.length; k++) {
    let map = []
    for (let i = 0; i < result.inputs.wightMap.size.y; i++) {
      let columns = []
      for (let j = 0; j < result.inputs.wightMap.size.x; j++) {
        // `${result.steps[k].map[i][j] != 0 && result.steps[k].map[i][j] < 254 ? `rgb(${result.steps[k].map[i][j] * 4}, 0, 0)` : ""}`
        columns.push(<div id={`${i*result.inputs.wightMap.size.y+j}`} 
          style={{backgroundColor: `${result.steps[k].map[i][j] > 1 && result.steps[k].map[i][j] < 254 ? `rgb(${255 - result.steps[k].map[i][j] * 8}, 0, 0)` : 
            result.steps[k].map[i][j] == 1 ? "rgb(0, 230, 0)" : result.steps[k].map[i][j] == 255 ? "rgb(0, 0, 230)" : ""}`}} 
          className={`min-w-[20px] min-h-[20px] border-2 border-solid border-black text-center m-[2px] p-[2px] ${result.steps[k].map[i][j] != 254 ? "text-white" : ""}`}>
            {result.steps[k].map[i][j]}</div>)
      }
      //map.push(rows)
      map.push(<div className="min-w-[30px] pt-[5px] pb-[5px] flex flex-col items-center">{columns}</div>)
    } 
    content.push(<div className="mr-[20px] border-2 border-solid border-black flex flex-col items-center gap-y-[5px] p-[5px]">
      <span className="text-xl">step {k}</span>
      <div className="flex">{map}</div>
      </div>)  
  }
  return content
}

export default App;