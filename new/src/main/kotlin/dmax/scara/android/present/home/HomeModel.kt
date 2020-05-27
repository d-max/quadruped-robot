package dmax.scara.android.present.home

import androidx.lifecycle.MutableLiveData
import dmax.scara.android.connect.Connector
import dmax.scara.android.present.home.HomeContract.Data
import dmax.scara.android.present.home.HomeContract.Event
import kotlinx.coroutines.Job
import kotlinx.coroutines.MainScope
import kotlinx.coroutines.cancel
import kotlinx.coroutines.launch

class HomeModel(
    private val connector: Connector
) : HomeContract.Model() {

    private val scope = MainScope()
    private var job: Job? = null

    override val data = MutableLiveData<Data>()

    override fun onCleared() {
        scope.cancel()
    }

    override fun event(e: Event) {
        when(e) {
            Event.Power -> {
                // was connecting and failed -> just turn off led
                if (data.value == Data.Error) {
                    data.value = Data.Disconnected
                    return
                }
                // is currently connecting -> cancel and turn off
                if (job?.isActive == true) {
                    job?.cancel()
                    data.value = Data.Disconnected
                    return
                }
                // previous completed -> normal behaviour
                if (connector.isConnected) {
                    disconnect()
                } else {
                    connect()
                }
            }
        }
    }

    private fun connect() {
        job?.cancel()
        job = scope.launch {
            data.value = Data.Connecting
            connector.connect()
            data.value = if (connector.isConnected) Data.Connected else Data.Error
        }
    }

    private fun disconnect() {
        job?.cancel()
        job = scope.launch {
            connector.disconnect()
            data.value = Data.Disconnected
        }
    }
}