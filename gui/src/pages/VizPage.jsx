import { useEffect, useState } from "react";
import { GraphPlotter } from "../components/GraphPlotter";
import "./VizPage.css"; 





export const VizPage = () => {
    const [data, setData] = useState([{x: 10, y: 10}]); 
    const interval = window.setInterval(() => setData([...data, {x: data[data.length - 1].x + 1, y: Math.random() * 100}]), 2000); 
     
    useEffect(() => {
        console.log(data); 
    }, [data])
    
    return (
        <div>

           

            <GraphPlotter data={data} />

            {/* Titulo */}
            <div className="d-flex my-4 ms-5  flex-column position-absolute align-items-start" style={{"width": "fit-content", "bottom": "0"}}>
                <h3>Proyecto de <strong>TdC</strong></h3>
                <p  style={{"lineHeight": "0"}}>
                    Balancín Hélice Motor - 2023i
                </p>
            </div>

        </div>
    )
}