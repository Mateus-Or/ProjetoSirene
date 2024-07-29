import { useState } from 'react';
import styles from './TimeForm.module.css';
import SubmitStyles from '../form/Submit.module.css';
import Input from '../form/Input';
import Submit from '../form/Submit';

function AlarmForm({ handleSubmit, alarmdate, onClose }) {
    const [alarm, setAlarm] = useState(alarmdate || {});

    function submit(e) {
        e.preventDefault();
        if (Object.values(alarm).some(value => value !== "")) {
            handleSubmit(alarm);
        } else {
            console.log("Por favor, preencha o campo do alarme.");
        }
    }

    function handleChange(e) {
        setAlarm({
            ...alarm,
            [e.target.name]: e.target.value,
            ativo: false
        });
    }

    return (
        <form>
            <Input type='time' text='Hora do Alarme' name='time' handleOnChange={handleChange} />
            <div className={styles.container_btn}>
                <Submit text="Cancelar" onClick={onClose} style={SubmitStyles.btn_cancelar} />
                <Submit text="Confirmar" onClick={submit} style={SubmitStyles.btn_confirmar} />
            </div>
        </form>
    );
}

export default AlarmForm;
