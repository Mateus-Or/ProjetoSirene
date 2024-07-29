
import styles from './TimeCard.module.css'
import {BsPencil, BsFillTrashFill} from 'react-icons/bs'
import ToggleSwitch from '../layouts/ToggleSwitch'
import Alarm from '../pages/Alarm'
import {useState} from 'react'
import Confirmacao from '../layouts/Confirmacao'


function TimeCard({id, time, handleRemove}){
    
    const remove = (e) =>{
        e.preventDefault()
        handleRemove(id)
    }
    const [isModalEditOpen, setIsModalEditOpen] = useState(false);

    const openModalEdit = () => {
      setIsModalEditOpen(true);
    };
    function closeModalEdit(){
        setIsModalEditOpen(false);
    }

    const [isModalConfOpen, setIsModalConfOpen] = useState(false);

    const openModalConf = () => {
      setIsModalConfOpen(true);
    };
    function closeModalConf(){
        setIsModalConfOpen(false);
    }
    
    
    return(
        <div className={styles.container_card}>

            <Alarm isOpen={isModalEditOpen} onClose={closeModalEdit} id={id}/>

            <Confirmacao isOpen={isModalConfOpen} onClose={closeModalConf} onConfirm={remove} text={"Deseja excluir o horário?"}/>
            <div className={styles.project_card}>

                 <ToggleSwitch id={id}/> 

                <h1  className={styles.category_text}>
                    {time}
                </h1>
                <div className={styles.project_card_actions}>
                    <button onClick={openModalEdit} className={styles.button_edit}>
                        <BsPencil/> Editar     
                    </button>
                    <button onClick={openModalConf} className={styles.button_remove}>
                        <BsFillTrashFill/> Excluir
                    </button>
                </div>
                
            </div>
            
        </div>
        
    )
}
export default TimeCard
