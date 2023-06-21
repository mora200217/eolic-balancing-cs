
import { Line } from 'react-chartjs-2';
import {
    Chart as ChartJS,
    CategoryScale,
    LinearScale,
    PointElement,
    LineElement,
    Title,
    Tooltip,
    Legend,
    TimeScale,
  } from 'chart.js';
import { useEffect, useState } from 'react';

  ChartJS.register(
    CategoryScale,
    LinearScale,
    PointElement,
    TimeScale,
    LineElement,
    Title,
    Tooltip,
    Legend
  );
  

export const GraphPlotter = (props) => {
    const [myData, setMyData] = useState(props.data || [{x: 5, y: 15}, {x: 10, y: 30}])

    useEffect(() => {
        
        setMyData(props.data.length <= 10 ? props.data : props.data.slice(props.data.length-11)); 
    }, [props])

    return ( 
        <div className='graph-wrapper'> 
        
            <Line
                width={"100%"}
                height={"100%"}
                options={{
                    maintainAspectRatio: false,
                    scales:{
                        x: {
                            type: 'linear',
                            display: false
                        },
                        y:{
                            type: 'linear',
                            display: true, 
                            min: -10,
                            max: 200

                        }
                    },
                    plugins:{
                        legend: {
                            display: false
                        },
                        title:{
                            display: true,
                            text: "Lectura Angular", 
                            
                        }

                    }
                }}
                data={ 
                    {
                        
                        datasets: [
                        {
                            data: myData,
                            borderColor: 'rgb(255, 99, 132)',
                            backgroundColor: 'rgba(255, 99, 132, 0.5)',
                        }
                        ],
                        
                    }
                }
                

            />
    </div>
    )
}