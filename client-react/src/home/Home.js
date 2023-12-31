import { Tab, Tabs, TabList, TabPanel } from 'react-tabs';
import 'react-tabs/style/react-tabs.css'; // Import the default Tabs styles
import { useFirebase } from "../FirebaseContext";
import Area from "./Area";
import style from './Home.module.scss'

const Home = () => {
    const { data, dbPath } = useFirebase()
    const dbPathAreas = dbPath + '/areas'

    // if (data && data.areas)
    //     Object.entries(data.areas).map(([id, area]) => { console.log("id:", id, "area:", area) })

    return (
        <div className={style.container}>
            {data.areas &&
                <Tabs className={style.tabs} style={{ borderBottom: 'none' }} >
                    <TabList className={style.tabList} >
                        {Object.entries(data.areas).map(([id, area]) => (
                            <Tab key={id} selectedClassName={style.selectedTab} className={`${style.tab} react-tabs__tab`}>{area.name}</Tab>
                        ))}
                    </TabList>
                    {Object.entries(data.areas).map(([id, area]) => (
                        <TabPanel key={id} className={style.tabPanel}>
                            <Area id={id} areaData={area} dbPathAreas={dbPathAreas} />
                        </TabPanel>
                    ))}
                </Tabs>}
            {!data.areas && <h1 className={style.h1}>{data.areas === null ? "Loading..." : "you have not taps yet!"}</h1>}
        </div>
    );
}

export default Home;