import React, { useState, useEffect } from 'react';
import styles from './DiasDeAlarme.module.css';

function DiasDeAlarme() {
  const [dias, setDias] = useState([]);

  useEffect(() => {
    // Carregando os dados do JSON a partir da URL local
    fetch('http://localhost:5000/diasdasemana')
      .then((response) => response.json())
      .then((data) => setDias(data));
  }, []);

  function handleClick(id){
    // Clonando o array para evitar mutações diretas no estado
    const updatedDias = [...dias];
    updatedDias[id].ativo = !updatedDias[id].ativo;

    console.log(dias)
    setDias(updatedDias);
    console.log('Estado Atualizado:', updatedDias);

    // Enviando os dados atualizados de volta para o servidor local (depende de sua configuração no servidor)
    fetch(`http://localhost:5000/diasdasemana/${id}`, {
      method: 'PATCH',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({ ativo: updatedDias[id].ativo }),
    });
  };

  return (
    <div className={styles.days_container}>
      {dias.map((dia, id) => (
        
        <button
          key={id}
          className={`${styles.day} ${dia.ativo ? styles.selected : ''}`}
          onClick={() => handleClick(id)}
        >
          {dia.dia.charAt(0)}
        </button>
      ))}
    </div>
  );
  
}

export default DiasDeAlarme;





