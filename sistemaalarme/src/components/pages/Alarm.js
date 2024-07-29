
import styles from './Alarm.module.css'
import {useState, useEffect} from 'react'
import AlarmForm from '../alarms/TimeForm'

function Projeto({isOpen, onClose, id}){
    const [alarm,   setAlarm] = useState([]) 
    const [message, setMessage] = useState()
    const [type, setType] = useState()

     
    useEffect(() =>{

        setTimeout(() => {
            fetch(`http://localhost:5000/alarms/${id}`, {
            method: 'GET',
            headers: {
                'Content-Type': 'application/json'
            },
            }).then(resp => resp.json())
            .then((data)=>{
                setAlarm(data)
                
            })
            .catch(erro => console.log(erro))
            },1500)
    },[id])

    function editPost(alarm){
        
        setMessage('')

        fetch(`http://localhost:5000/alarms/${alarm.id}`,{
            method:'PATCH',
            headers: {
                'Content-Type' : 'application/json'
            },
            body: JSON.stringify(alarm),
        })
        .then(resp => resp.json())
        .then((data) => {
            
            setAlarm(data)
            
            setType('success')
            setMessage('Projeto atualizado!')
            onClose()
            window.location.reload();
        })
        .catch(erro => console.log(erro))
    }  
    if(!isOpen){return null;}
    return(  
        <>
            <div className={styles.modal_overlay}>
                <div className={styles.modal}>
                
                <div>
                    <AlarmForm handleSubmit={editPost} alarmdate={alarm}/>
                </div>
                
                </div>
            </div>
        </>
    )
}

export default Projeto

    
