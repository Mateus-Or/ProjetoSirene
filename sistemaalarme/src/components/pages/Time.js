import { useLocation } from "react-router-dom"
import {useState,useEffect} from 'react'

import Message from "../layouts/Message"
import styles from './Time.module.css'
import Container from '../layouts/Container'
import TimeCard from "../alarms/TimeCard"
import Loading from "../layouts/Loading"
import NoTimes from "../layouts/NoTime"
import HorarioReal from "../layouts/HorarioReal"
import NewTime from "./NewTime"
import Confirmacao from "../layouts/Confirmacao"
import DiasDeAlarme from "../layouts/DiasDeAlarme"


function Time(){
    

    
        const [times, setTimes] = useState([])
        const [removeLoading, setRemoveLoading] = useState(false)
        const [timeMessage, setTimeMessage] = useState('')


        const location = useLocation()
        let message = ''
        if(location.state){
            message = location.state.message
        }

        useEffect(() => {

            setTimeout(() => {
                fetch('http://localhost:5000/alarms',{
                    method: 'GET',
                    headers: {
                    'Content-Type': 'application/json'
                }
                }).then(resp => resp.json())
                .then((data) => {
                
                setTimes(data)
                setRemoveLoading(true)
                })
                .catch((err) => console.log(err))
                
                
            }, 1500);
        },[])

        function removeTime(id){
            fetch(`http://localhost:5000/alarms/${id}` ,{
                method: 'DELETE',
                headers: {
                    'Content-Type':'application/json'
                },
            }).then(resp => resp.json())
            .then(() =>{
                setTimes(times.filter((time)=> time.id !== id))
                setTimeMessage('Alarme removido com sucesso!')
            })
            .catch(erro => console.log(erro))
        }
    const [isModalAddOpen, setIsModalAddOpen] = useState(false);

    const openModalAdd = () => {
      setIsModalAddOpen(true)
    }
    function closeModalAdd(){
        setIsModalAddOpen(false);
    }
    const [isModalConfOpen, setIsModalConfOpen] = useState(false);

    const openModalConf = () => {
      setIsModalConfOpen(true);
    };
    function closeModalConf(){
        setIsModalConfOpen(false);
    }
    return(
        <div className={styles.project_container}>
            <HorarioReal/>
            <NewTime onClose={closeModalAdd} isOpen={isModalAddOpen}/>
            
            {/*onConfirm deve ser substituido pela ação de tocar a sirene*/}
            <Confirmacao isOpen={isModalConfOpen} onClose={closeModalConf} onConfirm={closeModalConf} text={"Deseja tocar a sirene agora?"}/>

            <div className={styles.title_container}>
                <div className={styles.caixaBotoes}>
                    <button className={styles.botaoCriar} onClick={openModalAdd}>Adicionar Horario</button>
                    <button className={styles.botaoTocarAgora} onClick={openModalConf}>Tocar Agora</button>
                </div>
                <DiasDeAlarme/>
                <h1>Alarmes</h1>
            </div>
            
            {message && 
                <Message type='success' msg={message}/>
            }
            {timeMessage && 
                <Message type='success' msg={timeMessage}/>
            }
            <Container customClass='column'>
                {times.length > 0 &&
                    times.map((time) => (
                        <TimeCard id={time.id} key={time.id}  time={time.time}  handleRemove={removeTime}/>
                        
                    ))}

                    {!removeLoading && <Loading/>}
                    
                    {removeLoading && times.length===0 &&(
                        <NoTimes/>
                    )}
            </Container>
          
        </div>
    )
}

export default Time