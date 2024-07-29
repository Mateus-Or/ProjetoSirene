import React, { useState, useEffect } from 'react';
import styles from './HorarioReal.module.css'

const HorarioReal = () => {
  const [horario, setHorario] = useState(new Date());

  useEffect(() => {
    const intervalID = setInterval(() => {
      setHorario(new Date());
    }, 1000);

    return () => {
      clearInterval(intervalID);
    };
  }, []);

  const hora = horario.getHours();
  const minutos = horario.getMinutes();
  const segundos = horario.getSeconds();

  return (
    <div className={styles.container_horario}>
      <p className={styles.horario}>
        {hora < 10 ? `0${hora}` : hora}:
        {minutos < 10 ? `0${minutos}` : minutos}:
        {segundos < 10 ? `0${segundos}` : segundos}
      </p>
    </div>
  );
};

export default HorarioReal;
