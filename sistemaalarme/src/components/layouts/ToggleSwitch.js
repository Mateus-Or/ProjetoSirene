import React, { useState, useEffect } from 'react';
import styles from './ToggleSwitch.module.css';

function ToggleSwitch({ id }) {
  const [ativo, setAtivo] = useState(false);

  useEffect(() => {
    fetch(`http://localhost:5000/alarms/${id}`)
      .then((response) => response.json())
      .then((data) => {
        setAtivo(data.ativo);
      })
      .catch((error) => {
        console.error('Erro ao obter o estado do switch:', error);
      });
  }, [id]);

  const handleToggle = () => {
    const novoEstado = !ativo;
    fetch(`http://localhost:5000/alarms/${id}`, {
      method: 'PATCH',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({ ativo: novoEstado }),
    })
      .then(() => {
        setAtivo(novoEstado);
      })
      .catch((error) => {
        console.error('Erro ao atualizar o estado do switch:', error);
      });
  };

  return (
    <div>
      <button
        className={`${styles.btn} ${ativo ? styles.ativado : ''}`}
        onClick={handleToggle}
      >
        <div className={styles.slider}></div>
      </button>
    </div>
  );
}

export default ToggleSwitch;
