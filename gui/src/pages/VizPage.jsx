import { useDebugValue, useEffect } from "react";
import { ControlPanel } from "../components/ControlPanel"; 
import * as THREE from 'three';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls'
import { STLLoader } from 'three/examples/jsm/loaders/STLLoader'
import Stats from 'three/examples/jsm/libs/stats.module'
import modelA  from '../assets/ensamble_fijo.stl'; 
import { flattenJSON } from "three/src/animation/AnimationUtils";


export const VizPage = () => {

    
    // id.appendChild( renderer.domElement );

    useEffect( () => {
        const scene = new THREE.Scene();
        const width = window.innerWidth; 
        const height = window.innerHeight; 
        // const camera = new THREE.PerspectiveCamera( 75, window.innerWidth / window.innerHeight, 0.1, 1000 );
        const camera = new THREE.OrthographicCamera( width / - 2, width / 2, height / 2, height / - 2, 1, 1000 );
    
        const renderer = new THREE.WebGLRenderer( { antialias: true });
        renderer.setPixelRatio( window.devicePixelRatio );
        const light = new THREE.AmbientLight({})
        light.position.set(20, 20, 20)
        scene.add(light)

        
        const id = document.getElementById('myRenderer')
        const w = id.parentElement.width; 
        
        const _SCALE_FACTOR = 0.85; 

        renderer.setSize( window.innerWidth * _SCALE_FACTOR , window.innerHeight * _SCALE_FACTOR * 1.2);
        // console.log(id)
        id.appendChild(renderer.domElement); 
        const controls = new OrbitControls(camera, renderer.domElement)
        controls.enableDamping = true

        const axesHelper = new THREE.AxesHelper( 5 );
    scene.add( axesHelper );

        // const geometry = new THREE.BoxGeometry( 1, 1, 1 );
        const material = new THREE.MeshBasicMaterial( 
            {  
                wireframe: true,
                precision: 'highp',
                vertexColors: true 
            
            } 
            );
        // const cube = new THREE.Mesh( geometry, material );
        
        let mesh; 
        const loader = new STLLoader()
        // scene.add( cube );

        loader.load(
            // '../assets/ensamble_fijo.stl',
            modelA, 
            function (geometry) {
                mesh = new THREE.Mesh(geometry, material)
                
                mesh.rotation.x = -Math.PI/2; 
                mesh.rotation.z = Math.PI; 
                

                scene.add(mesh)
                camera.lookAt(mesh); 
            },
            (xhr) => {
                console.log((xhr.loaded / xhr.total) * 100 + '% loaded')
            },
            (error) => {
                console.log(error)
            }
        )

        camera.position.z = 100;
        camera.position.y = 300;
        
        

        function animate() {
            requestAnimationFrame(animate)
            controls.update()
            render()
        }
        
        function render() {
            renderer.render(scene, camera)
        }
        animate()

    }, [])

    return (
        <div>
            <section className="" style={{
                width: "100vw",
                height: "100vh",
                bottom: 0

            }}>
            <div className="" style={{
                position: "absolute",
                left: 0,
                bottom: 0,
                border: "solid #fffff 2px", 
                zIndex: 0, 

                // background: 'red' 

            }} id="myRenderer"/>
            </section>
            

           

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