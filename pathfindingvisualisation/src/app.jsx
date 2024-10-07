import React from "react";
import { useState, useEffect } from 'react';
import './tailwind.css'
const COLUMNS = 10
const ROWS = 10

function App() 
{
  const [Grid, setGrid] = useState();

  

  return (
    <div>
      <button className="w-fit rounded-lg bg-blue-700 p-[5px] m-[10px]" onClick={() => {
        let input = document.createElement('input');
        input.type = 'file';
        input.accept = 'application/json';
        input.onchange = _ => {
          let file = Array.from(input.files)[0]
          console.log(file)
          // fs.readFile(file, 'utf-8', (err, data) => {
          //   console.log(data)
          // })
        };
        input.click()
      }}>
        LoadFile
      </button>

      <button onClick={() => {setGrid(MakeGrid())}}>MakeGrid</button>
      {Grid == null ? null : <div className="flex gap-x-[5px] border-2 border-solid border-black w-fit">{Grid}</div>}
      
    </div>
  )
}

function MakeGrid()
{
  let content = []
  for (let i = 0; i < COLUMNS; i++) {
    let rows = []
    for (let j = 0; j < ROWS; j++) {
      rows.push(<div className="min-w-[20px] min-h-[20px] border-2 border-solid border-black"></div>)
    }
    content.push(<div className="min-w-[30px] pt-[5px] pb-[5px] flex flex-col items-center gap-y-[5px]">{rows}</div>)
  }
  return content
}

export default App;