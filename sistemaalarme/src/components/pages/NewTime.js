import styles from './NewTime.module.css'

import TimeForm from '../alarms/TimeForm'

import { useNavigate } from 'react-router-dom'

function NewTime({ isOpen, onClose }){

    const history = useNavigate()

    
    function createPost(alarm){

        fetch('http://localhost:5000/alarms',{
            method: 'POST',
            headers:{
                'Content-type': 'application/json'
            },
            body: JSON.stringify(alarm)
        }).then(resp => resp.json())
        .then((data)=>{  
            history('/times', { state: { message: 'Alarme programado com sucesso!'}})
            window.location.reload();
        })
        .catch(erro => console.log(erro))
        
    }
    if (!isOpen) return null;
    return(
        <div className={styles.caixa_newProject}>
            <div className={styles.modal_overlay}>
                <div className={styles.modal}>
                    <h1 className={styles.titulo}>Criar Alarme</h1>
                    <TimeForm handleSubmit={createPost} onClose={onClose} textobtn='Criar projeto'/>
                </div>
            </div>
        </div>
    )
}

export default NewTime