import styles from './LinkButton.module.css'

import { Link } from 'react-router-dom'

function LinkButton({text, to}){
    return(
        <Link to={to} className={styles.botaoCriar}> {text}
        </Link>
    )
}

export default LinkButton