import styles from './Loading.module.css'

function Loading(){
    return ( 
            <div className={styles.caixa_loading}>
                <div className={styles.loading}></div>
                <p>Loading</p>
            </div>
    )
        
    
}

export default Loading